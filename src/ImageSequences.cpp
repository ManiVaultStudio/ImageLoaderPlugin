#include "ImageSequences.h"
#include "ImageSequence.h"

#include <QDebug>
#include <QDirIterator>

ImageSequences::ImageSequences()
{
}

ImageSequences::ImageSequences(const ImageSequences &other) :
	_directory(other._directory) {
}

ImageSequences::~ImageSequences()
{
}

QDebug operator<<(QDebug dbg, const ImageSequences &sequences)
{
	// dbg << sequence.size() << ", " << sequence.type() << ", " << sequence.directory();

	return dbg;
}

QString ImageSequences::directory() const
{
	return _directory;
}

void ImageSequences::load(const QString & directory)
{
	_directory = directory;

	const auto splitDirectories = false;
	const auto splitImageType = true;

	QDirIterator it(directory, QStringList() << "*.jpg", QDir::Files, QDirIterator::Subdirectories);

	QMap<QString, ImageSequence> sequences;

	while (it.hasNext()) {
		const auto filePath = it.next();

		QFileInfo fileinfo(filePath);
		/*
		QImageReader reader(it.next());

		const auto imageSize = reader.size();
		const auto imageType = fileinfo.completeSuffix();
		const auto directory = fileinfo.absoluteDir().absolutePath();
		const auto sequence = ImageSequence(imageSize, "jpg", directory);

		
		auto sequenceName =
		if (splitDirectories == true) {
			sequenceName.append);
		}

		if (splitImageType == true) {
			sequenceName.append(fileinfo.completeSuffix());
		}

		if (!sequences.contains(sequenceName)) {
			sequences.insert(sequenceName, );
		}

		sequences[sequenceName].addFile(it.next());


		// qDebug() << imageSize << sequenceName;
		*/
	}
}
