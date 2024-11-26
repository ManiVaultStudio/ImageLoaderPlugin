#include "ImageCollectionsModel.h"
#include "ImageLoaderPlugin.h"

#include "Common.h"

#include "Application.h"

#include "util/Miscellaneous.h"

#include <QDebug>
#include <QPainter>
#include <QIcon>
#include <QtConcurrent>

using namespace mv::util;

ImageCollectionsModel::ImageCollectionsModel(ImageLoaderPlugin* imageLoaderPlugin) :
    _imageLoaderPlugin(imageLoaderPlugin),
    _root(new TreeItem(nullptr)),
    _selectionModel(this)
{
    //_settings = _imageLoaderPlugin->getSetting("Cache").toMap();

    _settingsTimer.setSingleShot(true);
    _settingsTimer.setInterval(100);

    connect(&_settingsTimer, &QTimer::timeout, this, &ImageCollectionsModel::saveSettings);
}

ImageCollectionsModel::~ImageCollectionsModel()
{
    delete _root;
}

int ImageCollectionsModel::rowCount(const QModelIndex& parent /* = QModelIndex() */) const
{
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        return _root->childCount();

	return static_cast<TreeItem*>(parent.internalPointer())->childCount();
}

int ImageCollectionsModel::columnCount(const QModelIndex& parent) const
{
    return ult(ImageCollection::Image::Column::End) + 1;
}

QVariant ImageCollectionsModel::data(const QModelIndex& index, int role /* = Qt::DisplayRole */) const
{
    if (!index.isValid())
        return {};

    if (index.parent() == QModelIndex()) {
        auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

        switch (static_cast<ImageCollection::Column>(index.column())) {
            case ImageCollection::Column::Name:
                return imageCollection->getName(role);

            case ImageCollection::Column::DatasetName:
                return imageCollection->getDatasetName(role);

            case ImageCollection::Column::FileNames:
                return imageCollection->getFileNames(role);

            case ImageCollection::Column::ImageType:
                return imageCollection->getImageType(role);

            case ImageCollection::Column::ImageFormat:
                return imageCollection->getImageFormat(role);

            case ImageCollection::Column::ToGrayscale:
                return imageCollection->getToGrayscale(role);

            case ImageCollection::Column::NoImages:
                return imageCollection->getNoImages(role);

            case ImageCollection::Column::NoSelectedImages:
                return imageCollection->getNoSelectedImages(role);

            case ImageCollection::Column::DimensionTag:
                return imageCollection->getDimensionTag(role);

            case ImageCollection::Column::IsMultiPage:
                return imageCollection->getIsMultiPage(role);

            case ImageCollection::Column::SourceSize:
                return imageCollection->getSourceSize(role);

            case ImageCollection::Column::TargetSize:
                return imageCollection->getTargetSize(role);

            case ImageCollection::Column::TargetWidth:
                return imageCollection->getTargetWidth(role);

            case ImageCollection::Column::TargetHeight:
                return imageCollection->getTargetheight(role);

            case ImageCollection::Column::NoPoints:
                return imageCollection->getNoPoints(role);

            case ImageCollection::Column::NoDimensions:
                return imageCollection->getNoDimensions(role);

            case ImageCollection::Column::Memory:
                return imageCollection->getMemoryConsumption(role);

            case ImageCollection::Column::Directory:
                return imageCollection->getDirectory(role);

            case ImageCollection::Column::Type:
                return imageCollection->getType(role);

            case ImageCollection::Column::SubsamplingType:
                return imageCollection->getSubsampling().getType(role);

            case ImageCollection::Column::SubsamplingRatio:
                return imageCollection->getSubsampling().getRatio(role);

            case ImageCollection::Column::SubsamplingFilter:
                return imageCollection->getSubsampling().getFilter(role);

            case ImageCollection::Column::SubsamplingNumberOfLevels:
                return imageCollection->getSubsampling().getNumberOfLevels(role);

            case ImageCollection::Column::SubsamplingLevelFactor:
                return imageCollection->getSubsampling().getLevelFactor(role);

            case ImageCollection::Column::Conversion:
                return imageCollection->getConversion(role);

            case ImageCollection::Column::AddCoordinatesPoints:
                return imageCollection->getAddCoordinatesPoints(role);

            case ImageCollection::Column::MirrorHorizontal:
                return imageCollection->getMirrorHorizontal(role);

            case ImageCollection::Column::MirrorVertical:
                return imageCollection->getMirrorVertical(role);
        }
    }
    else {
        auto image = static_cast<ImageCollection::Image*>((void*)index.internalPointer());

        switch (static_cast<ImageCollection::Image::Column>(index.column())) {
            case ImageCollection::Image::Column::ShouldLoad:
                return image->getShouldLoad(role);

            case ImageCollection::Image::Column::FileName:
                return image->getFileName(role);

            case ImageCollection::Image::Column::DimensionName:
                return image->getDimensionName(role);

            case ImageCollection::Image::Column::FilePath:
                return image->getFilePath(role);
        }
    }

    return {};
}

bool ImageCollectionsModel::setData(const QModelIndex& index, const QVariant& value, int role /*= Qt::EditRole*/)
{
    QModelIndexList affectedIndices;

    affectedIndices << index;

    QVariantMap setting;

    if (index.parent() == QModelIndex()) {
        auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

        const auto directory = imageCollection->getDirectory(Qt::EditRole).toString();

        if (_settings.contains(directory))
            setting = _settings[directory].toMap();

        //qDebug() << setting;

        const auto column = static_cast<ImageCollection::Column>(index.column());

        auto updateTargetSize = [&index, &imageCollection, &affectedIndices]() {
            const auto subsamplingType      = static_cast<ImageCollection::SubSampling::Type>(imageCollection->getSubsampling().getType(Qt::EditRole).toInt());
            const auto subsamplingRatio     = imageCollection->getSubsampling().getRatio(Qt::EditRole).toFloat();
            const auto sourceSize           = imageCollection->getSourceSize(Qt::EditRole).toSize();

            auto targetSize = sourceSize;

            if (subsamplingType == ImageCollection::SubSampling::Type::Resample) {
                targetSize.setWidth(std::max(static_cast<std::int32_t>(subsamplingRatio * sourceSize.width()), 1));
                targetSize.setHeight(std::max(static_cast<std::int32_t>(subsamplingRatio * sourceSize.height()), 1));
            }

            imageCollection->setTargetSize(targetSize);

            affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::TargetSize));
            affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::TargetWidth));
            affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::TargetHeight));
            affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::NoPoints));
            affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::NoDimensions));
            affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::Memory));
        };

        switch (role)
        {
            case Qt::EditRole:
            {
                switch (column) {
                    case ImageCollection::Column::Name:
                    {
                        imageCollection->setName(value.toString());

                        setSettingByPath(setting, "/Name", value.toString());
                        break;
                    }

                    case ImageCollection::Column::DatasetName:
                    {
                        imageCollection->setDatasetName(value.toString());

                        setSettingByPath(setting, "/DatasetName", value.toString());
                        break;
                    }

                    case ImageCollection::Column::FileNames:
                        break;

                    case ImageCollection::Column::DimensionTag:
                    {
                        imageCollection->setDimensionTag(value.toString());

                        setSettingByPath(setting, "/DimensionTag", value.toString());
                        break;
                    }

                    case ImageCollection::Column::Type:
                    {
                        imageCollection->setType(static_cast<ImageData::Type>(value.toInt()));

                        affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::NoPoints));
                        affectedIndices << index.siblingAtColumn(ult(ImageCollection::Column::NoDimensions));

                        setSettingByPath(setting, "/Type", value.toInt());
                        break;
                    }

                    case ImageCollection::Column::SubsamplingType:
                    {
                        imageCollection->getSubsampling().setType(static_cast<ImageCollection::SubSampling::Type>(value.toInt()));

                        setSettingByPath(setting, "/Subsampling/Type", value.toInt());
                        updateTargetSize();
                        break;
                    }

                    case ImageCollection::Column::SubsamplingRatio:
                    {
                        imageCollection->getSubsampling().setRatio(value.toFloat());
                        updateTargetSize();

                        setSettingByPath(setting, "/Subsampling/Ratio", value.toFloat());
                        break;
                    }

                    case ImageCollection::Column::SubsamplingFilter:
                    {
                        imageCollection->getSubsampling().setFilter(static_cast<ImageCollection::SubSampling::Filter>(value.toInt()));

                        setSettingByPath(setting, "/Subsampling/Filter", value.toInt());
                        break;
                    }

                    case ImageCollection::Column::SubsamplingNumberOfLevels:
                    {
                        imageCollection->getSubsampling().setNumberOfLevels(value.toInt());

                        setSettingByPath(setting, "/Subsampling/NumberOfLevels", value.toInt());
                        break;
                    }

                    case ImageCollection::Column::SubsamplingLevelFactor:
                    {
                        imageCollection->getSubsampling().setLevelFactor(value.toInt());

                        setSettingByPath(setting, "/Subsampling/LevelFactor", value.toInt());
                        break;
                    }

                    case ImageCollection::Column::Conversion:
                    {
                        imageCollection->setConversion(value.toString());

                        setSettingByPath(setting, "/Conversion/SHA", value.toString());
                        break;
                    }

                    case ImageCollection::Column::AddCoordinatesPoints:
                    {
                        imageCollection->setAddCoordinatesPoints(value.toBool());

                        setSettingByPath(setting, "/AddCoordinatesPoints", value.toBool());
                        break;
                    }

                    case ImageCollection::Column::MirrorHorizontal:
                    {
                        imageCollection->setMirrorHorizontal(value.toBool());

                        setSettingByPath(setting, "/Mirror/Horizontal", value.toBool());
                        break;
                    }

                    case ImageCollection::Column::MirrorVertical:
                    {
                        imageCollection->setMirrorVertical(value.toBool());

                        setSettingByPath(setting, "/Mirror/Vertical", value.toBool());
                        break;
                    }

                    default:
                        break;
                }

                break;
            }

            case Qt::CheckStateRole:
            {
                switch (column) {
                    case ImageCollection::Column::ToGrayscale:
                    {
                        imageCollection->setToGrayscale(value.toBool());
                        updateTargetSize();

                        setSettingByPath(setting, "/ToGrayscale", value.toBool());
                        break;
                    }

                    default:
                        break;
                }

                break;
            }

            default:
                break;
        }

        _settings[directory] = setting;
    }
    else {
        auto image = static_cast<ImageCollection::Image*>((void*)index.internalPointer());

        const auto directory = image->getImageCollection()->getDirectory(Qt::EditRole).toString();

        if (_settings.contains(directory))
            setting = _settings[directory].toMap();

        const auto column = static_cast<ImageCollection::Image::Column>(index.column());

        switch (role)
        {
            case Qt::EditRole:
            {
                switch (column) {
                    case ImageCollection::Image::Column::ShouldLoad:
                    {
                        image->setShouldLoad(value.toBool());

                        affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::NoSelectedImages));
                        affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::NoPoints));
                        affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::NoDimensions));
                        affectedIndices << index.parent().siblingAtColumn(ult(ImageCollection::Column::Memory));

                        setSettingByPath(setting, QString("/Images/%2/%3/ShouldLoad").arg(image->getImageCollection()->getName(Qt::EditRole).toString(), QString::number(index.row())), value.toBool());
                        break;
                    }

                    case ImageCollection::Image::Column::FileName:
                        break;

                    case ImageCollection::Image::Column::DimensionName:
                    {
                        image->setDimensionName(value.toString());

                        setSettingByPath(setting, QString("/Images/%2/%3/DimensionName").arg(image->getImageCollection()->getName(Qt::EditRole).toString(), QString::number(index.row())), value.toString());

                        break;
                    }

                    case ImageCollection::Image::Column::FilePath:
                    default:
                        break;
                }

                break;
            }

            case Qt::CheckStateRole:
            {
                switch (column) {
                    case ImageCollection::Image::Column::ShouldLoad:
                    {
                        setData(index, value.toBool(), Qt::EditRole);
                        break;
                    }

                    case ImageCollection::Image::Column::FileName:
                    case ImageCollection::Image::Column::DimensionName:
                    case ImageCollection::Image::Column::FilePath:
                        break;

                    default:
                        break;
                }

                break;
            }

            default:
                break;
        }

        _settings[directory] = setting;
    }

    for (auto affectedIndex : affectedIndices)
        emit dataChanged(affectedIndex, affectedIndex);

    return true;
}

QVariant ImageCollectionsModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/) const
{
    if (orientation == Qt::Horizontal) {
        switch (role)
        {
            case Qt::DecorationRole:
            {
                switch (static_cast<ImageCollection::Image::Column>(section)) {
                    case ImageCollection::Image::Column::DimensionName:
                        return mv::Application::getIconFont("FontAwesome").getIcon("pen", Qt::darkGray);

                    default:
                        break;
                }

                return {};
            }

            case Qt::DisplayRole:
            {
                switch (static_cast<ImageCollection::Column>(section)) {
                    case ImageCollection::Column::Name:
                        return "Name";

                    case ImageCollection::Column::DatasetName:
                        return "Dataset name";

                    case ImageCollection::Column::FileNames:
                        return "File name(s)";

                    case ImageCollection::Column::ImageType:
                        return "Image type";

                    case ImageCollection::Column::ImageFormat:
                        return "Image format";

                    case ImageCollection::Column::ToGrayscale:
                        return "Grayscale";

                    case ImageCollection::Column::NoImages:
                        return "#Images";

                    case ImageCollection::Column::NoSelectedImages:
                        return "#Selected images";

                    case ImageCollection::Column::DimensionTag:
                        return "Dimension tag";

                    case ImageCollection::Column::IsMultiPage:
                        return "Multi-page";

                    case ImageCollection::Column::SourceSize:
                        return "Source size";

                    case ImageCollection::Column::TargetSize:
                        return "Target size";

                    case ImageCollection::Column::TargetWidth:
                        return "Width";

                    case ImageCollection::Column::TargetHeight:
                        return "Height";

                    case ImageCollection::Column::NoPoints:
                        return "#Points";

                    case ImageCollection::Column::NoDimensions:
                        return "#Dimensions";

                    case ImageCollection::Column::Memory:
                        return "Memory";

                    case ImageCollection::Column::Directory:
                        return "Directory";

                    case ImageCollection::Column::Type:
                        return "Load as";

                    case ImageCollection::Column::SubsamplingType:
                        return "Subsampling type";

                    case ImageCollection::Column::SubsamplingRatio:
                        return "Subsampling ratio";

                    case ImageCollection::Column::SubsamplingFilter:
                        return "Subsampling filter";

                    case ImageCollection::Column::SubsamplingNumberOfLevels:
                        return "Subsampling number of levels";

                    case ImageCollection::Column::SubsamplingLevelFactor:
                        return "Subsampling level factor";

                    case ImageCollection::Column::Conversion:
                        return "Conversion";

                    case ImageCollection::Column::AddCoordinatesPoints:
                        return "Add 2D coordinates points";

                    case ImageCollection::Column::MirrorHorizontal:
                        return "Mirror horizontally";

                    case ImageCollection::Column::MirrorVertical:
                        return "Mirror vertically";

                    default:
                        break;
                }

                switch (static_cast<ImageCollection::Image::Column>(section)) {
                    case ImageCollection::Image::Column::ShouldLoad:
                        return "";

                    case ImageCollection::Image::Column::FileName:
                        return "Filename";

                    case ImageCollection::Image::Column::DimensionName:
                        return "Dimension name";

                    case ImageCollection::Image::Column::FilePath:
                        return "File path";

                    default:
                        break;
                }

                break;
            }

            case Qt::ToolTipRole:
            {
                switch (static_cast<ImageCollection::Column>(section)) {
                    case ImageCollection::Column::Name:
                        return "The name of the high-dimensional dataset, click to edit";

                    case ImageCollection::Column::DatasetName:
                        return "The name of the dataset";

                    case ImageCollection::Column::FileNames:
                        return "The file name(s)";

                    case ImageCollection::Column::ImageType:
                        return "The type of images in the scanned image collection";

                    case ImageCollection::Column::NoImages:
                        return "The number of images in the collection";

                    case ImageCollection::Column::NoSelectedImages:
                        return "The number of selected images in the collection";

                    case ImageCollection::Column::SourceSize:
                        return "The size of the images on disk";

                    case ImageCollection::Column::TargetSize:
                        return "The size of the images when loaded as high-dimensional data";

                    case ImageCollection::Column::TargetWidth:
                        return "The width of the images when loaded as high-dimensional data";

                    case ImageCollection::Column::TargetHeight:
                        return "The height of the images when loaded as high-dimensional data";

                    case ImageCollection::Column::NoPoints:
                        return "The number of points in the high-dimensional data";

                    case ImageCollection::Column::NoDimensions:
                        return "The number of dimensions in the high-dimensional data";

                    case ImageCollection::Column::Memory:
                        return "Memory consumption of the high-dimensional data";

                    case ImageCollection::Column::Directory:
                        return "The top-level directory where the images were found";

                    case ImageCollection::Column::Type:
                        return "How to interpret the images as high-dimensional data";

                    case ImageCollection::Column::SubsamplingType:
                        return "Type of image subsampling";

                    case ImageCollection::Column::SubsamplingRatio:
                        return "The amount of subsampling";

                    case ImageCollection::Column::SubsamplingFilter:
                        return "The subsampling filter to use";

                    case ImageCollection::Column::SubsamplingNumberOfLevels:
                        return "The number of image pyramid subsampling levels to use";

                    case ImageCollection::Column::SubsamplingLevelFactor:
                        return "The level factor in case of image pyramid subsampling";

                    case ImageCollection::Column::ToGrayscale:
                        return "Whether all image channels are combined in to one (grayscale)";

                    case ImageCollection::Column::Conversion:
                        return "Conversion";

                    case ImageCollection::Column::AddCoordinatesPoints:
                        return "Whether to create an additional 2D coordinates points dataset";

                    case ImageCollection::Column::MirrorHorizontal:
                        return "Whether to mirror horizontally";

                    case ImageCollection::Column::MirrorVertical:
                        return "Whether to mirror vertically";

                    default:
                        break;
                }

                switch (static_cast<ImageCollection::Image::Column>(section)) {
                    case ImageCollection::Image::Column::ShouldLoad:
                        return "Whether to load the image or not";

                    case ImageCollection::Image::Column::FileName:
                        return "Name of the image file";

                    case ImageCollection::Image::Column::DimensionName:
                        return "Name of the dimension in the high-dimensional dataset, click to edit";

                    case ImageCollection::Image::Column::FilePath:
                        return "The absolute file path of the image";

                    default:
                        break;
                }

                break;
            }
        }
    }

    return {};
}

Qt::ItemFlags ImageCollectionsModel::flags(const QModelIndex& index) const
{
    auto flags = static_cast<Qt::ItemFlags>(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);

    auto imageCollection = static_cast<ImageCollection*>((void*)index.internalPointer());

    if (index.parent() == QModelIndex()) {
        switch (static_cast<ImageCollection::Column>(index.column())) {
            case ImageCollection::Column::Name:
                break;

            case ImageCollection::Column::ToGrayscale:
            {
                flags |= Qt::ItemIsUserCheckable;

                if (index != QModelIndex())
                    flags |= Qt::ItemIsEditable;

                if (imageCollection->getNumberOfChannelsPerPixel(Qt::EditRole).toInt() == 1)
                    flags &= ~Qt::ItemIsEnabled;

                break;
            }
            
            /*
            case ImageCollection::Column::DimensionTag:
            {
                if (index != QModelIndex())
                    flags |= Qt::ItemIsEditable;

                break;
            }
            */

            case ImageCollection::Column::Directory:
            case ImageCollection::Column::SourceSize:
            case ImageCollection::Column::TargetSize:
            case ImageCollection::Column::TargetWidth:
            case ImageCollection::Column::TargetHeight:
            default:
                break;
        }
    }
    else {
        switch (static_cast<ImageCollection::Image::Column>(index.column())) {
            case ImageCollection::Image::Column::ShouldLoad:
            {
                flags |= Qt::ItemIsUserCheckable;

                if (index != QModelIndex())
                    flags |= Qt::ItemIsEditable;

                break;
            }

            case ImageCollection::Image::Column::FileName:
                break;

            case ImageCollection::Image::Column::DimensionName:
            {
                flags |= Qt::ItemIsEditable;
                break;
            }

            case ImageCollection::Image::Column::FilePath:
            default:
                break;
        }
    }

    return flags;
}

QModelIndex ImageCollectionsModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
        return {};

    TreeItem* parentItem;

    if (!parent.isValid())
        parentItem = _root;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    if (auto childItem = parentItem->child(row))
        return createIndex(row, column, childItem);

    return {};
}

QModelIndex ImageCollectionsModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return {};

    TreeItem *childItem     = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem    = childItem->getParentItem();

    if (parentItem == _root)
        return {};

    return createIndex(parentItem->row(), 0, parentItem);
}

void ImageCollectionsModel::clear()
{
    beginResetModel();
    {
        _root->removeAllChildren();
    }
    endResetModel();
}

//std::int32_t restoreSettings(ImageCollectionsModel* model, ImageLoaderPlugin* imageLoaderPlugin) {
//    qDebug() << "Running in thread:" << QThread::currentThread();
//
//    QVariantList settings;
//
//    //model->beginResetModel();
//    //{
//        /*
//        for (int rowIndex = 0; rowIndex < model->rowCount(QModelIndex()); rowIndex++)
//        {
//            const auto imageCollectionIndex = model->index(rowIndex, 0);
//            const auto name                 = model->data(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::Name)), Qt::EditRole).toString();
//            const auto noImages             = model->data(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::NoImages)), Qt::EditRole).toInt();
//
//            QVariant setting;
//
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::Name)), );
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::DatasetName)), getValueByPath(setting, "/DatasetName", name).toString());
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::DimensionTag)), getValueByPath(setting, "/DimensionTag", "").toString());
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::Type)), getValueByPath(setting, "/Type", ImageData::Type::Stack).toInt());
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingType)), getValueByPath(setting, "/Subsampling/Type", 0).toInt());
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingRatio)), getValueByPath(setting, "/Subsampling/Ratio", 0.5f).toFloat());
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingFilter)), getValueByPath(setting, "/Subsampling/Filter", 0).toInt());
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingNumberOfLevels)), getValueByPath(setting, "/Subsampling/NumberOfLevels", 0).toInt());
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::SubsamplingLevelFactor)), getValueByPath(setting, "/Subsampling/LevelFactor", 0).toInt());
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::AddCoordinatesPoints)), getValueByPath(setting, "/AddCoordinatesPoints", 0).toBool());
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::MirrorHorizontal)), getValueByPath(setting, "/Mirror/Horizontal", 0).toBool());
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::MirrorVertical)), getValueByPath(setting, "/Mirror/Vertical", 0).toBool());
//
//            const auto conversion = getValueByPath(setting, "/Conversion/SHA", "").toString();
//
//            model->setData(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::Conversion)), getValueByPath(setting, "/Conversion/SHA", 0).toString());
//
//            for (int imageRow = 0; imageRow < noImages; imageRow++) {
//                const auto dimensionName    = getValueByPath(setting, QString("%1/Images/%2/%3/DimensionName").arg(name, QString::number(imageRow)), "").toString();
//                const auto shouldLoad       = getValueByPath(setting, QString("%1/Images/%2/%3/ShouldLoad").arg(name, QString::number(imageRow)), true).toBool();
//
//                if (!dimensionName.isEmpty())
//                    model->setData(model->index(imageRow, 0, imageCollectionIndex).siblingAtColumn(ult(ImageCollection::Image::Column::DimensionName)), dimensionName);
//
//                model->setData(model->index(imageRow, 0, imageCollectionIndex).siblingAtColumn(ult(ImageCollection::Image::Column::ShouldLoad)), shouldLoad);
//            }
//        }
//        */
//    //}
//    //model->endResetModel();
//
//    return model->rowCount(QModelIndex());
//}

void ImageCollectionsModel::insert(int row, const QVector<ImageCollection*>& imageCollections)
{
    if (imageCollections.empty())
        return;

    beginResetModel();
    {
        beginInsertRows(QModelIndex(), static_cast<int>(_root->childCount()), static_cast<int>(_root->childCount() + imageCollections.size() - 1));
        {
            for (auto& imageCollection : imageCollections) {
                //QVariant setting;

                //const auto directory = imageCollection->getDirectory(Qt::EditRole).toString();

                //if (_settings.contains(directory))
                //    setting = _settings[directory];

                //const auto name = imageCollection->getName(Qt::EditRole).toString();

                //imageCollection->setName(getValueByPath(setting, "/Name", name).toString());
                //imageCollection->setDatasetName(getValueByPath(setting, "/DatasetName", name).toString());
                //imageCollection->setDimensionTag(getValueByPath(setting, "/DimensionTag", "").toString());
                //imageCollection->setType(static_cast<ImageData::Type>(getValueByPath(setting, "/Type", ImageData::Type::Stack).toInt()));
                //imageCollection->getSubsampling().setType(static_cast<ImageCollection::SubSampling::Type>(getValueByPath(setting, "/Subsampling/Type", 0).toInt()));
                //imageCollection->getSubsampling().setRatio(getValueByPath(setting, "/Subsampling/Ratio", 0.5f).toFloat());
                //imageCollection->getSubsampling().setFilter(static_cast<ImageCollection::SubSampling::Filter>(getValueByPath(setting, "/Subsampling/Filter", 0).toInt()));
                //imageCollection->getSubsampling().setNumberOfLevels(getValueByPath(setting, "/Subsampling/NumberOfLevels", 0).toInt());
                //imageCollection->getSubsampling().setLevelFactor(getValueByPath(setting, "/Subsampling/LevelFactor", 0).toInt());
                //imageCollection->setAddCoordinatesPoints(getValueByPath(setting, "/AddCoordinatesPoints", 0).toBool());
                //imageCollection->setMirrorHorizontal(getValueByPath(setting, "/Mirror/Horizontal", 0).toBool());
                //imageCollection->setMirrorVertical(getValueByPath(setting, "/Mirror/Vertical", 0).toBool());

                _root->appendChild(imageCollection);
            }
        }
        endInsertRows();
    }
    endResetModel();
}

void ImageCollectionsModel::guessDimensionNames(const QModelIndex& imageCollectionIndex)
{
    if (imageCollectionIndex.parent() != QModelIndex())
        return;

    auto imageCollection = static_cast<ImageCollection*>((void*)imageCollectionIndex.internalPointer());

    const auto guessedDimensionNames = imageCollection->guessDimensionNames();

    for (auto child : imageCollection->_children)
        setData(index(imageCollection->_children.indexOf(child), ult(ImageCollection::Image::Column::DimensionName), imageCollectionIndex), guessedDimensionNames[imageCollection->_children.indexOf(child)]);
}

QString ImageCollectionsModel::getSettingsPrefix(const QModelIndex& index) const
{
    const auto imageCollectionIndex = index.parent().isValid() ? index.parent() : index;
    const auto directory            = data(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::Directory)), Qt::EditRole).toString();
    const auto fileNames            = data(imageCollectionIndex.siblingAtColumn(ult(ImageCollection::Column::FileNames)), Qt::EditRole).toStringList();

    return QString("Cache/" + QDir::fromNativeSeparators(directory) + "/" + fileNames.first());
}

void ImageCollectionsModel::selectAll(const QModelIndex& parent)
{
    if (parent.parent() != QModelIndex())
        return;

    for (std::int32_t row = 0; row < rowCount(parent); row++) {
        setData(index(row, ult(ImageCollection::Image::Column::ShouldLoad), parent), true);
    }
}

void ImageCollectionsModel::selectNone(const QModelIndex& parent)
{
    if (parent.parent() != QModelIndex())
        return;

    for (std::int32_t row = 0; row < rowCount(parent); row++) {
        setData(index(row, ult(ImageCollection::Image::Column::ShouldLoad), parent), false);
    }
}

void ImageCollectionsModel::invertSelection(const QModelIndex& parent)
{
    if (parent.parent() != QModelIndex())
        return;

    auto imageCollection = static_cast<ImageCollection*>((void*)parent.internalPointer());

    for (std::int32_t row = 0; row < rowCount(parent); row++) {
        const auto imageIndex = index(row, ult(ImageCollection::Image::Column::ShouldLoad), parent);
        const auto shouldLoad = data(imageIndex, Qt::EditRole).toBool();

        setData(imageIndex, !shouldLoad);
    }
}

void ImageCollectionsModel::selectPercentage(const QModelIndex& parent, const float& selectionProbability)
{
    if (parent.parent() != QModelIndex())
        return;

    auto imageCollection = static_cast<ImageCollection*>((void*)parent.internalPointer());

    const auto probability      = std::clamp(selectionProbability, 0.0f, 1.0f);
    const auto shouldLoadColumn = ult(ImageCollection::Image::Column::ShouldLoad);

    for (std::int32_t row = 0; row < rowCount(parent); row++) {
        const auto random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

        setData(index(row, shouldLoadColumn, parent), random < selectionProbability);
    }
}

void ImageCollectionsModel::requestSaveSettings()
{
    if (!_settingsTimer.isActive())
        _settingsTimer.start();
}

void ImageCollectionsModel::setSettingByPath(QVariantMap root, const QString& path, const QVariant& value)
{
    //root = setValueByPath(root, path, value).toMap();
    //requestSaveSettings();
}

void ImageCollectionsModel::saveSettings() const
{
    //_imageLoaderPlugin->setSetting("Cache", _settings);
}
