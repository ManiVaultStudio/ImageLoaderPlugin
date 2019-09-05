#include "ImageStackScanner.h"

#include <QDebug>
#include <QDir>

ImageStackScanner::ImageStackScanner(QSettings* settings) :
	ImageScanner(settings)
{
}

void ImageStackScanner::scan()
{
	QMap<QString, QStringList> _stacks;

	emit beginScan();

	emit message("Scanning for images...");

	//_stacks.clear();

	/*
	if (_imageStacks.stacks().size() == 0) {
		_ui->infoLineEdit->setText("No image stacks were found, try changing the directory");
	}
	else {
		_ui->infoLineEdit->setText(QString("Found %1 image stack(s)").arg(_imageStacks.stacks().size()));

		_ui->stacksComboBox->addItems(_imageStacks.stacks().keys());

		_ui->datasetNameLabel->setEnabled(true);
		_ui->datasetNameLineEdit->setEnabled(true);
		_ui->stacksLabel->setEnabled(true);
		_ui->stacksComboBox->setEnabled(true);
		_ui->stacksComboBox->setEnabled(true);
	}

	reset();

	scanDir(_directory);

	if (noImages() > 0) {
		emit message(QString("Found %1 images").arg(noImages()));
	}
	else {
		emit message("No images were found, try changing the directory, image type or dimensions");
	}
	

	scanDir(_imageStack.directory());

	emit _imageStack.endScan();*/
}

void ImageStackScanner::scanDir(const QString& directory)
{
	/*
	auto subDirectories = QDir(directory);

	subDirectories.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	const auto dirList = subDirectories.entryList();

	for (int i = 0; i < dirList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(subDirectories.absolutePath()).arg(dirList.at(i));

		qDebug() << "Found directory: " << dirList.at(i);

		scanDir(path);
	}

	auto imageFiles = QDir(directory);

	imageFiles.setFilter(QDir::Files);

	auto nameFilters = QStringList();

	foreach(QString imageType, _imageStack.imageTypes())
	{
		nameFilters << "*." + imageType;
	}

	imageFiles.setNameFilters(nameFilters);

	const auto fileList = imageFiles.entryList();

	for (int i = 0; i < fileList.size(); ++i)
	{
		const auto path = QString("%1/%2").arg(imageFiles.absolutePath()).arg(fileList.at(i));

		QImageReader imageReader(path);

		// qDebug() << imageReader.size();

		const auto size = imageReader.size();

		if (size.width() > 0 && size.height() > 0) {
			const auto sizeString = QString("%1x%2").arg(QString::number(imageReader.size().width()), QString::number(imageReader.size().height()));

			if (!_stacks.contains(sizeString)) {
				_stacks.insert(sizeString, QStringList());
			}

			_stacks[sizeString] << path;
		}

		scanDir(path);
	}
	*/
}