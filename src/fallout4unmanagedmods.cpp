#include "fallout4unmanagedmods.h"

Fallout4UnmangedMods::Fallout4UnmangedMods(const GameGamebryo *game) : GamebryoUnmangedMods(game)
{}

Fallout4UnmangedMods::~Fallout4UnmangedMods()
{}

QStringList Fallout4UnmangedMods::mods(bool onlyOfficial) const {
  QStringList result;

  QStringList pluginList = game()->primaryPlugins();
  QStringList otherPlugins = game()->DLCPlugins();
  otherPlugins.append(game()->CCPlugins());
  for (QString plugin : otherPlugins) {
    pluginList.removeAll(plugin);
  }
  QDir dataDir(game()->dataDirectory());
  for (const QString &fileName : dataDir.entryList({"*.esp", "*.esl", "*.esm"})) {
    if (!pluginList.contains(fileName, Qt::CaseInsensitive)) {
      if (!onlyOfficial || pluginList.contains(fileName, Qt::CaseInsensitive)) {
        result.append(fileName.chopped(4)); // Trims the extension off
      }
    }
  }

  return result;
}

QStringList Fallout4UnmangedMods::secondaryFiles(const QString &modName) const {
  // Archives in Fallout 4 are BA2 and not BSA
  QStringList archives;
  QDir dataDir = game()->dataDirectory();
  for (const QString &archiveName : dataDir.entryList({modName + "*.ba2"})) {
    archives.append(dataDir.absoluteFilePath(archiveName));
  }
  return archives;
}

QString Fallout4UnmangedMods::displayName(const QString &modName) const
{
  // Unlike in earlier games, in Fallout 4 the file name doesn't correspond to the public name
  if (modName.compare("dlcrobot", Qt::CaseInsensitive) == 0) {
    return "Automatron";
  } else if (modName.compare("dlcworkshop01", Qt::CaseInsensitive) == 0) {
    return "Wasteland Workshop";
  } else if (modName.compare("dlccoast", Qt::CaseInsensitive) == 0) {
    return "Far Harbor";
  } else if (modName.compare("dlcworkshop02", Qt::CaseInsensitive) == 0) {
    return "Contraptions Workshop";
  } else if (modName.compare("dlcworkshop03", Qt::CaseInsensitive) == 0) {
    return "Vault-Tec Workshop";
  } else if (modName.compare("dlcnukaworld", Qt::CaseInsensitive) == 0) {
    return "Nuka-World";
  } else if (modName.compare("dlcultrahighresolution", Qt::CaseInsensitive) == 0) {
    return "Ultra High Resolution Texture Pack";
  } else {
    return modName;
  }
}
