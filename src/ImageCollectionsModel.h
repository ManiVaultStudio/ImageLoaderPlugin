#pragma once

#include "ImageCollection.h"

#include "ImageData/Images.h"

#include <QAbstractItemModel>
#include <QItemSelectionModel>

/**
 * Image collections model class
 *
 * TODO: Write description
 *
 * @author Thomas Kroes
 */
class ImageCollectionsModel : public QAbstractItemModel
{
public: // Enumerations

	/** TODO: Write description */
	enum class Column {
		DatasetName,				/** The name of the dataset */
		ImageType,					/** The type of image(s) */
		NoImages,					/** Number of images in the collection */
		NoSelectedImages,			/** Number of selected images in the collection */
		SourceSize,					/** Size of the source image(s) */
		TargetSize,					/** Size of the target image(s) */
		TargetWidth,				/** Target width of the image(s) */
		TargetHeight,				/** Target height of the image(s) */
		Type,						/** Load as image sequence (0) or image stack (1) */
		SubsamplingEnabled,			/** Whether subsampling is enabled */
		SubsamplingRatio,			/** Subsampling ratio */
		SubsamplingFilter,			/** Subsampling filter */
		ToGrayscale,				/** Whether to convert the images to grayscale */
		Directory,					/** Directory */
		Images,						/** Images */

		Start = DatasetName,		/** Column start */
		End = Images				/** Column End */
	};

public: // Construction/destruction

	/** Default constructor */
	ImageCollectionsModel();

	/** Default destructor */
	~ImageCollectionsModel();

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
	 * Sets the data value for the given model index and data role
	 * @param index Model index
	 * @param value Data value in variant form
	 * @param role Data role
	 * @return Whether the data was properly set or not
	 */
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

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

	/**
	 * Returns the model for the given row and column and parent model index
	 * @param row Row
	 * @param column Column
	 * @param parent Parent model index
	 * @return Model index
	 */
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
	
	/**
	 * Returns the parent model index of the given model index
	 * @param index Model index
	 * @return Parent model index
	 */
	QModelIndex parent(const QModelIndex& index) const;

public: // Miscellaneous

	/** Returns the selection model */
	QItemSelectionModel& selectionModel() { return _selectionModel; }

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

	/** Get root tree item */
	TreeItem* rootItem() { return _root; };

private:
	//std::vector<ImageCollection>	_imageCollections;		/** Images collections */
	TreeItem*						_root;					/** Root tree item */
	QItemSelectionModel				_selectionModel;		/** Selection model */
};