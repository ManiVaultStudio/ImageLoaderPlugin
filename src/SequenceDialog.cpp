#include "SequenceDialog.h"

#include "ui_SequenceDialog.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

SequenceDialog::SequenceDialog()
	:
	_ui{std::make_unique<Ui::SequenceDialog>()},
	_treeModel(this)
{
	_ui->setupUi(this);

	resize(640, 480);

	connect(_ui->directoryPushButton, &QPushButton::clicked, [this]
	{
		PickDirectory();
	});

	connect(_ui->imageTypeComboBox, &QComboBox::currentTextChanged, [this]
	{
		UpdateImageTree();
	});

	connect(_ui->imageWidthSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val) {
		UpdateImageTree();
	});

	connect(_ui->imageHeightSpinBox, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, [this](int val) {
		UpdateImageTree();
	});

	connect(_ui->lineEdit, &QLineEdit::textChanged, [this]
	{
		
		//_FilteredImagesTreeModel.setFilterRegExp(QRegExp(_ui->lineEdit->text(), Qt::CaseInsensitive, QRegExp::FixedString));
		UpdateImageTree();
		//_ui->imagesTreeView->setRootIndex(_FilteredImagesTreeModel.mapFromSource(_treeModel.index(_ui->directoryLineEdit->text())));
		//_FilteredImagesTreeModel.setFilterFixedString(_ui->lineEdit->text());
	});

	PickDirectory();

	/*
	connect(_ui->imageResolutionSquareCheckBox, &QPushButton::clicked, [this]
	{
		PickDirectory();
	});
	*/

	_ui->imageTypeComboBox->addItem("jpg");
	_ui->imageTypeComboBox->addItem("png");
	_ui->imageTypeComboBox->addItem("bmp");
}

SequenceDialog::~SequenceDialog()
{
}

void SequenceDialog::PickDirectory()
{
	_directory = QFileDialog::getExistingDirectory(Q_NULLPTR, "Choose image directory");

	if (!_directory.isNull() || !_directory.isEmpty()) {
		_ui->directoryLineEdit->setText(_directory);
		UpdateImageTree();
	}
}

void SequenceDialog::UpdateImageTree() {
	qDebug() << "Updating image tree";
	qDebug() << "Filter on " << _ui->imageTypeComboBox->currentText();

	_treeModel.setRootPath(_directory);
	//_treeModel.setFilter(QDir::NoDotAndDotDot | QDir::Files);
	_FilteredImagesTreeModel.setDynamicSortFilter(true);
	_FilteredImagesTreeModel.setRecursiveFilteringEnabled(true);
	_FilteredImagesTreeModel.setSourceModel(&_treeModel);
	_FilteredImagesTreeModel.setFilterWildcard(_ui->lineEdit->text());
	_FilteredImagesTreeModel.setFilterCaseSensitivity(Qt::CaseSensitivity::CaseInsensitive);
	
	_FilteredImagesTreeModel.setFilterKeyColumn(-1);

	qDebug() << _FilteredImagesTreeModel.rowCount();
	// qDebug() << _FilteredImagesTreeModel.mapFromSource(_treeModel.index(_directory));

	

	//_ui->imagesTreeView->setRootIndex(_treeModel.index(_directory));
	_ui->imagesTreeView->setRootIndex(_FilteredImagesTreeModel.mapFromSource(_treeModel.index(_directory)));

	_ui->imagesTreeView->setModel(&_FilteredImagesTreeModel);
}
