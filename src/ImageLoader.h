#pragma once

#include "Settings.h"
#include "ImageCollections.h"
#include "Payload.h"
#include "SubsampleSettings.h"
#include "ColorSettings.h"

#include <QThread>
#include <QString>

namespace fi {
	#include <FreeImage.h>
}

class ImageLoader : public QThread, public Settings {
	Q_OBJECT

public:
	ImageLoader(const ImageCollectionType& type);

	ImageCollectionType type() const;

	SubsampleSettings& subsampleImageSettings();
	ColorSettings& colorSettings();

	void load(std::shared_ptr<ImageCollections> scanned);
	void run() override;

public:
	QString datasetName() const;
	void setDatasetName(const QString& datasetName, const bool& forceUpdate = false);
	std::shared_ptr<ImageCollections> scanned() const;
	void setScanned(std::shared_ptr<ImageCollections> scanned);

private:
	void loadBitmap(fi::FIBITMAP* bitmap, Payload* payload, const QString& imageFilePath, const QString& dimensionName = "");

signals:
	void settingsChanged();
	void datasetNameChanged(const QString&);
	void beginLoad();
	void endLoad(std::shared_ptr<Payload> payload);
	void imageLoaded(const QString &imageFilePath, const int& done, const int& total);
	void message(const QString& message);

private:
	ImageCollectionType					_type;
	QString								_datasetName;
	SubsampleSettings					_subsampleSettings;
	ColorSettings						_colorSettings;
	std::shared_ptr<ImageCollections>	_scanned;
};