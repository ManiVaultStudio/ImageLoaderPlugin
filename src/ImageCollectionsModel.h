#pragma once

#include "ImageCollection.h"

#include "ImageData/Images.h"

#include <QAbstractListModel>

/**
 * Image collections model class
 *
 * TODO: Write description
 *
 * @author Thomas Kroes
 */
class ImageCollectionsModel : public QAbstractListModel
{
public: // Enumerations

	/** TODO: Write description */
	enum class Column {
		DatasetName,			/** The name of the dataset */
		NoImages,				/** Number of images in the collection */
		Grayscale,				/** Whether to convert the images to grayscale */
		SourceSize,				/** Size of the source image(s) */
		TargetSize,				/** Size of the target image(s) */
		SearchDir,				/** Initial directory where the search was started */

		Start = DatasetName,	/** Column start */
		End = SearchDir			/** Column End */
	};

	/** Get string representation of column enumeration */
	static QString columnName(const Column& column) {
		switch (column) {
			case Column::DatasetName:
				return "Name";

			case Column::NoImages:
				return "#Images";

			case Column::Grayscale:
				return "Grayscale";

			case Column::SourceSize:
				return "Source size";

			case Column::TargetSize:
				return "Target size";

			case Column::SearchDir:
				return "Search directory";

			default:
				return QString();
		}

		return QString();
	}

public: // Construction

	/** Default constructor */
	ImageCollectionsModel();

public: // Inherited MVC

	/**
	 * Returns the number of color maps in the model
	 * @param parent Parent index
	 */
	int rowCount(const QModelIndex& parent /* = QModelIndex() */) const override;

	/**
	 * Returns the number of columns in the model given the parent model index
	 * @param parent Parent model index
	 * @return Number of columns in the model given the parent model index
	 */
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	/**
	 * Returns model data for the given index
	 * @param index Index
	 * @param role The data role
	 */
	QVariant data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const override;

	/**
	 * Returns the header data for the given section, orientation and data role
	 * @param section Model section
	 * @param orientation Orientation (e.g. horizontal or vertical)
	 * @param role Data role
	 * @return Header data in variant form
	 */
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

	/**
	 * Returns the item flags for the given model index
	 * @param index Model index
	 * @return Item flags for the index
	 */
	Qt::ItemFlags flags(const QModelIndex& index) const override;

public: // Miscellaneous

	/** Clears the model */
	void clear();

	/**
	 * Inserts image collections into the model at the specified row
	 * @param row Row at which to insert the image collections
	 * @param imageCollections Image collections to insert
	 */
	void insert(int row, const std::vector<ImageCollection>& imageCollections);

	/**
	 * Returns an image collection by given row index
	 * @param row Row index
	 */
	const ImageCollection* imageCollection(const int& row) const;

private:
	std::vector<ImageCollection>	_imageCollections;		/** Images collections */
};