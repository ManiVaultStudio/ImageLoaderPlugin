#pragma once

#include "ImageCollection.h"

#include "ImageData/Images.h"

#include <QAbstractListModel>
#include <QItemSelectionModel>

/**
 * Image collections model class
 *
 * TODO: Write description
 *
 * @author Thomas Kroes
 */
class ImagesModel : public QAbstractListModel
{
public: // Enumerations

	/** TODO: Write description */
	enum class Column {
		Name,				/** The name of the dataset */
		FilePath,			/** Number of images in the collection */
		ShouldLoad,			/** Whether to load the image or not */

		Start = Name,		/** Column start */
		End = ShouldLoad	/** Column End */
	};

	/** Get string representation of column enumeration */
	static QString columnName(const Column& column) {
		switch (column) {
			case Column::Name:
				return "Name";

			case Column::FilePath:
				return "File path";

			case Column::ShouldLoad:
				return "Load";

			default:
				return QString();
		}

		return QString();
	}

public: // Construction

	/** Default constructor */
	ImagesModel();

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

public: // Miscellaneous

	/** Returns the selection model */
	QItemSelectionModel& selectionModel() { return _selectionModel; }

	/**
	 * Sets the image collection from which this model retrieves the images
	 * @param images Images to insert
	 */
	void setImageCollection(ImageCollection* imageCollection);

private:
	ImageCollection*		_imageCollection;		/** Pointer to image collection */
	QItemSelectionModel		_selectionModel;		/** Selection model */
};