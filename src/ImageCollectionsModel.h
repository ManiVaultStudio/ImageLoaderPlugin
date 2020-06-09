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
	void insert(int row, const std::vector<ImageCollection*>& imageCollections);

	/** Get root tree item */
	TreeItem* rootItem() { return _root; };
	
public: // Image selection

	/**
	 * Selects all images in the image collection
	 * @param index Image collection model index
	 */
	void selectAll(const QModelIndex& index);

	/**
	 * Deselects all images in the image collection
	 * @param index Image collection model index
	 */
	void selectNone(const QModelIndex& index);

	/**
	 * Inverts the image selection in the image collection
	 * @param index Image collection model index
	 */
	void invertSelection(const QModelIndex& index);

	/**
	 * Selects a percentage of images in the image collection
	 * @param index Image collection model index
	 * @param selectionProbability Probability that an image will be selected [0 - 1]
	 */
	void selectPercentage(const QModelIndex& index, const float& selectionProbability);

private:
	//std::vector<ImageCollection>	_imageCollections;		/** Images collections */
	TreeItem*						_root;					/** Root tree item */
	QItemSelectionModel				_selectionModel;		/** Selection model */
};