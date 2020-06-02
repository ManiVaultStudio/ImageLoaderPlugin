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

	/**
	 * Model columns
	 * Defines the image collections model columns
	 */
	enum class Column {
		Name,				/** The name of the image (collection) */
		SearchDir,			/** Initial directory where the search was started */
		FilePath,			/** Absolute file path */
		Index,				/** Image index */
		ShouldLoad,			/** Whether the image should be loaded */
		SourceSize,			/** Size of the source image */
		TargetSize,			/** Size of the target image */

		Start = Index,		/** Column start */
		End = TargetSize	/** Column End */
	};

	/** Get string representation of column enumeration */
	static QString columnName(const Column& column) {
		switch (column) {
			case Column::Name:
				return "Name";

			case Column::SearchDir:
				return "Search directory";

			case Column::FilePath:
				return "File path";

			case Column::Index:
				return "Image index";

			case Column::ShouldLoad:
				return "Load";

			case Column::SourceSize:
				return "Source size";

			case Column::TargetSize:
				return "Target size";

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

	/**
	 * Inserts an image collection into the model at the specified row give the parent index
	 * @param row Row at which to insert the layer
	 * @param imageCollection Image collection to insert
	 * @return Whether the image collection was successfully inserted
	 */
	bool insert(int row, const ImageCollection& imageCollection);

	/**
	 * Returns an image collection by given row index
	 * @param row Row index
	 */
	const ImageCollection* imageCollection(const int& row) const;

private:
	QVector<ImageCollection>	_imageCollections;		/** Images collections */
};