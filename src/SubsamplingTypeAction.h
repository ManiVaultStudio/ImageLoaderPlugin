#pragma once

#include <actions/OptionAction.h>

class ImageLoaderPlugin;

using namespace mv::gui;

class SubsamplingTypeAction : public OptionAction
{
public:
    SubsamplingTypeAction(QObject* parent, ImageLoaderPlugin& imageLoaderPlugin);

private:
    void setCurrentIndexSilently(const std::int32_t& currentIndex);
    void updateRows();
    void dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight, const QList<int>& roles = QList<int>());
    void updateStateFromModel();

private:
    ImageLoaderPlugin&      _imageLoaderPlugin;

};