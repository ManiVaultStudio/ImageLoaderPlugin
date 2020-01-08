#pragma once

#include "Settings.h"
#include "Scanned.h"
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

	void load(std::shared_ptr<Scanned> scanned);
	void run() override;

public:
	QString datasetName() const;
	void setDatasetName(const QString& datasetName, const bool& forceUpdate = false);
	std::shared_ptr<Scanned> scanned() const;
	void setScanned(std::shared_ptr<Scanned> scanned);

private:
	void loadBitmap(fi::FIBITMAP* bitmap, Payload* payload, const QString& imageFilePath, const QString& dimensionName = "");

signals:
	void settingsChanged();
	void datasetNameChanged(const QString& datasetName);
	void beginLoad();
	void endLoad(std::shared_ptr<Payload> payload);
	void imageLoaded(const QString &imageFilePath, const int& done, const int& total);
	void message(const QString& message);

private:
	ImageCollectionType			_type;					/*!< Type of image collection e.g. sequence, stack */
	QString						_datasetName;			/*!< Name of the data set */
	SubsampleSettings			_subsampleSettings;		/*!< Image subsample settings */
	ColorSettings				_colorSettings;			/*!< Image color settings */
	std::shared_ptr<Scanned>	_scanned;				/*!< Scanned image collections */
};