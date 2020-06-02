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

public: // Construction

	/** Default constructor */
	ImageCollectionsModel();

public: // Inherited MVC

	/**
	 * Returns the the number of model columns
	 * @param parent Parent index
	 */
	int columnCount(const QModelIndex& parent) const override;

	/**
	 * Returns the number of color maps in the model
	 * @param parent Parent index
	 */
	int rowCount(const QModelIndex& parent /* = QModelIndex() */) const override;

	/**
	 * Returns model data for the given index
	 * @param index Index
	 * @param role The data role
	 */
	QVariant data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const override;

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