#pragma once

#include <memory>

#include <QDialog>
#include <QStackedWidget>

#include "ImageSequence.h"

class QVBoxLayout;
class QComboBox;

class ImageLoader;
class ImageSequenceWidget;
class ImageStackWidget;

class StackedWidget : public QStackedWidget
{
	QSize sizeHint() const override;
	QSize minimumSizeHint() const override;

public:
	/*
	void addWidget(QWidget* pWidget)
	{
		pWidget->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
		QStackedWidget::addWidget(pWidget);
	}

	void onCurrentChanged(int index)
	{
		QWidget* pWidget = widget(index);
		Q_ASSERT(pWidget);
		pWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		pWidget->adjustSize();
		adjustSize();
	}
	*/
};

class ImageLoaderDialog : public QDialog
{
public:
	ImageLoaderDialog(ImageLoader *imageLoader);
	~ImageLoaderDialog();

private slots:
	void onTypeChanged();
	/*
	void onBecameDirty();
	void onBeginScan();
	void onEndScan();
	void onMessage(const QString &message);
	void onDirectoryChanged(const QString &directory);
	void onLoadSequence();
	void onImageWidthChanged(int width);
	void onImageHeightChanged(int height);
	void onScan();
	void onPickDirectory();
	void onImageTypeChanged(const QString &imageType);
	void onBeginLoad();
	void onEndLoad();
	*/

private:
	ImageLoader*							_imageLoaderPlugin;
	std::unique_ptr<QVBoxLayout>			_mainLayout;
	std::unique_ptr<QComboBox>				_typesComboBox;
	std::unique_ptr<StackedWidget>			_pagesStackedWidget;
	std::unique_ptr<ImageSequenceWidget>	_imageSequenceWidget;
	std::unique_ptr<ImageStackWidget>		_imageStackWidget;
};

