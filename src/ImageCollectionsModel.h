#pragma once

#include "ImageCollection.h"

#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
#include <QItemSelectionModel>

class ImageLoaderPlugin;

/**
 * Image collections model class
 *
 * @author Thomas Kroes
 */
class ImageCollectionsModel : public QAbstractItemModel
{
public: // Filter model

    /**
     * Filter model for image collections model
     *
     * Allows end-user to filter based on dataset name (case-insensitive)
     *
     * @author Thomas Kroes
     */
    class Filter : public QSortFilterProxyModel {
    public:
        /** Default constructor */
        Filter() :
            QSortFilterProxyModel(),
            _filter()
        {
        }

        /**
         * Returns whether a give row with give parent is filtered out (false) or in (true)
         * @param row Row index
         * @param parent Parent index
         */
        bool filterAcceptsRow(int row, const QModelIndex& parent) const
        {
            if (_filter.isEmpty())
                return true;

            if (parent == QModelIndex()) {
                const auto fileNamesIndex   = sourceModel()->index(row, ult(ImageCollection::Column::FileNames));
                const auto fileNames        = sourceModel()->data(fileNamesIndex, Qt::EditRole).toStringList();

                for (auto fileName : fileNames) {
                    if (fileName.contains(_filter, Qt::CaseInsensitive))
                        return true;
                }

                const auto datasetNameIndex = sourceModel()->index(row, ult(ImageCollection::Column::Name));
                const auto datasetName      = sourceModel()->data(datasetNameIndex, Qt::EditRole).toString();

                return datasetName.contains(_filter, Qt::CaseInsensitive);
            }

            return true;
        }

        /** Returns the filter string */
        QString filter() const {
            return _filter;
        }

        /**
         * Sets the filter string
         * @param filter Filter string
         */
        void setFilter(const QString& filter) {
            if (filter == _filter)
                return;

            _filter = filter;

            invalidateFilter();
        }

    private:
        QString _filter;    /** Filter string */
    };

public: // Construction/destruction

    /**
     * Constructor
     * @param imageLoaderPlugin Pointer to image loader plugin
     */
    ImageCollectionsModel(ImageLoaderPlugin* imageLoaderPlugin);

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
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    /**
     * Returns the parent model index of the given model index
     * @param index Model index
     * @return Parent model index
     */
    QModelIndex parent(const QModelIndex& index) const override;

public: // Miscellaneous

    /** Returns the selection model */
    const QItemSelectionModel& selectionModel() const { return _selectionModel; }
    QItemSelectionModel& selectionModel() { return _selectionModel; }

    /** Clears the model */
    void clear();

    /**
     * Inserts image collections into the model at the specified row
     * @param row Row at which to insert the image collections
     * @param imageCollections Image collections to insert
     */
    void insert(int row, const QVector<ImageCollection*>& imageCollections);

    /** Get root tree item */
    TreeItem* getRootItem() { return _root; };

    /**
     * Guesses the image dimension names
     * @param index Image collection model index
     */
    void guessDimensionNames(const QModelIndex& imageCollectionIndex);

    /**
     * Returns the image collection settings prefix given an arbitrary model index
     * @param index Model index of the image collection
     * @return Settings prefix for the image collection
     */
    QString getSettingsPrefix(const QModelIndex& index) const;

public: // Image selection

    /**
     * Selects all images in the image collection
     * @param index Image collection model index
     */
    void selectAll(const QModelIndex& parent);

    /**
     * Deselects all images in the image collection
     * @param index Image collection model index
     */
    void selectNone(const QModelIndex& parent);

    /**
     * Inverts the image selection in the image collection
     * @param index Image collection model index
     */
    void invertSelection(const QModelIndex& parent);

    /**
     * Selects a percentage of images in the image collection
     * @param index Image collection model index
     * @param selectionProbability Probability that an image will be selected [0 - 1]
     */
    void selectPercentage(const QModelIndex& parent, const float& selectionProbability);

public:

    /** If possible save settings right away, otherwise defer it */
    void requestSaveSettings();

private:

    /**
     * Set child \p value of \root by \p path
     * @param root Root element
     * @param path Path to the value
     * @param value Value to set
     */
    void setSettingByPath(QVariantMap root, const QString& path, const QVariant& value);

    /** Save settings directly */
    void saveSettings() const;

public:

    using QAbstractItemModel::beginResetModel;
    using QAbstractItemModel::endResetModel;

private:
    ImageLoaderPlugin*      _imageLoaderPlugin;     /** Image loader plugin instance */
    TreeItem*               _root;                  /** Root tree item */
    QItemSelectionModel     _selectionModel;        /** Selection model */
    QVariantMap             _settings;              /** Persistent settings for image collections */
    QTimer                  _settingsTimer;         /** Timer for preventing unnecessary settings saves */
};