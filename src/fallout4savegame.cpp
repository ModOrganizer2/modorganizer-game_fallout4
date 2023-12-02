#include "fallout4savegame.h"

#include <Windows.h>

#include "gamefallout4.h"

Fallout4SaveGame::Fallout4SaveGame(QString const &fileName, GameFallout4 const* game) : GamebryoSaveGame(fileName, game, true)
{
  FileWrapper file(getFilepath(), "FO4_SAVEGAME");

  FILETIME creationTime;
  fetchInformationFields(file, m_SaveNumber, m_PCName, m_PCLevel, m_PCLocation, creationTime);

  // A file time is a 64-bit value that represents the number of 100-nanosecond intervals that have elapsed since 12:00 A.M. January 1, 1601 Coordinated Universal Time (UTC). So we need to convert that to something useful
  SYSTEMTIME ctime;
  ::FileTimeToSystemTime(&creationTime, &ctime);

  setCreationTime(ctime);
}

void Fallout4SaveGame::fetchInformationFields(
  FileWrapper& file,
  unsigned long& saveNumber,
  QString& playerName,
  unsigned short& playerLevel,
  QString& playerLocation,
  FILETIME& creationTime) const
{
  file.skip<unsigned long>();  // Header size
  file.skip<uint32_t>();  // Header version
  file.read(saveNumber);
  file.read(playerName);

  unsigned long temp;
  file.read(temp);
  playerLevel = static_cast<unsigned short>(temp);
  file.read(playerLocation);

  QString ignore;
  file.read(ignore);  // Playtime as ASCII hh.mm.ss
  file.read(ignore);  // Race name (i.e. BretonRace)

  file.skip<unsigned short>();  // Player gender (0 = male)
  file.skip<float>(2);  // Experience gathered, experience required

  file.read(creationTime);
}

std::unique_ptr<GamebryoSaveGame::DataFields> Fallout4SaveGame::fetchDataFields() const
{
  FileWrapper file(getFilepath(), "FO4_SAVEGAME"); // 10 bytes

  {
    QString dummyName, dummyLocation;
    unsigned short dummyLevel;
    unsigned long dummySaveNumber;
    FILETIME dummyTime;

    fetchInformationFields(file, dummySaveNumber, dummyName, dummyLevel, dummyLocation, dummyTime);
  }

  QString ignore;
  std::unique_ptr<DataFields> fields = std::make_unique<DataFields>();

  fields->Screenshot = file.readImage(384, true);

  uint8_t saveGameVersion = file.readChar();
  file.read(ignore);  // Game version
  file.skip<uint32_t>();  // Plugin info size

  fields->Plugins = file.readPlugins();
  if (saveGameVersion >= 68) {
    fields->LightPlugins = file.readLightPlugins();
  }

  return fields;
}