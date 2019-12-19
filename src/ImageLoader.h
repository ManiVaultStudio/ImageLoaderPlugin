#pragma once

#include "ImageCollections.h"
#include "Payload.h"
#include "SubsampleSettings.h"
#include "ColorSettings.h"

#include <QThread>
#include <QString>

namespace fi {
	#include <FreeImage.h>
}

class ImageLoader : public QThread {
	Q_OBJECT

public:
	ImageLoader(const ImageCollectionType& type);

	ImageCollectionType type() const;

	SubsampleSettings& subsampleImageSettings();
	ColorSettings& colorSettings();

	void load(std::shared_ptr<ImageCollections> scanned);
	void run() override;

public:
	QVariant setting(const QString& name, const QVariant& defaultValue = QVariant()) const;
	void setSetting(const QString& name, const QVariant& value);
	QString datasetName() const;
	void setDatasetName(const QString& datasetName);
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

protected:
	QSettings							_settings;

private:
	ImageCollectionType					_type;
	QString								_datasetName;
	SubsampleSettings					_subsampleSettings;
	ColorSettings						_colorSettings;
	std::shared_ptr<ImageCollections>	_scanned;
};