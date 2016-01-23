#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	i_pic(0),
	ui(new Ui::MainWindow),
	cameras(QCameraInfo::availableCameras())
{
	ui->setupUi(this);
	comboBox_camera = ui->comboBox_camera;

	int camera_num = cameras.size();
	for (int i = 0; i < camera_num; ++i) {
		comboBox_camera->addItem(cameras[i].description());
	}
	if (camera_num != 0) {
		comboBox_camera->setCurrentIndex(0);
	} else {
		// TODO: alert user that no cameras were detected.
	}
	updateCameraCaptureSettings();

	QString status_str = "Status: ";

	status_str += QString::number(camera_num) + " camera(s)\n";

	camera->start();

	ui->label_status->setText(status_str);

	// Set up object connections
	QObject::connect(	ui->button_save_directory,	&QPushButton::clicked,
						this,						&MainWindow::fetchSaveDir);

	QObject::connect(	ui->button_capture_single,	&QPushButton::clicked,
						this,						&MainWindow::takePicture);
	QObject::connect(	ui->button_captue_play,		&QPushButton::clicked,
						this,						&MainWindow::startSequence);
	QObject::connect(	ui->button_capture_stop,	&QPushButton::clicked,
						this,						&MainWindow::stopSequence);
}

MainWindow::~MainWindow()
{
	delete ui;
	// TODO: delete other things?
}

void MainWindow::takePicture()
{
	QCameraImageCapture* capturer = new QCameraImageCapture(camera);
	camera->setCaptureMode(QCamera::CaptureStillImage);
	camera->searchAndLock();
	capturer->capture(ui->lineEdit_save_directory->text() + "/" + QString::number(i_pic));
	i_pic++;
	camera->unlock();
}

void MainWindow::startSequence()
{
	QCameraImageCapture* capturer = new QCameraImageCapture(camera);
	camera->setCaptureMode(QCamera::CaptureStillImage);
	camera->searchAndLock();
	capturer->capture(ui->lineEdit_save_directory->text() + "/" + QString::number(i_pic));
	i_pic++;
	QObject::connect(	capturer,	&QCameraImageCapture::imageSaved,
						this,		&MainWindow::startSequence);
}

void MainWindow::stopSequence()
{
	camera->unlock();
}

void MainWindow::updateCameraCaptureSettings()
{
	QCameraInfo current_camera_info = cameras[comboBox_camera->currentIndex()];

	// Update viewfinder
	camera = new QCamera(current_camera_info);
	camera->setViewfinder(ui->viewfinder);

	// Update focus settings
	QCameraFocus* focus = camera->focus();
	bool isFocusAvailable = focus->isAvailable();

	// Update ISO settings
	QCameraExposure* exposure = camera->exposure();
	bool isExposureAvailable = exposure->isAvailable();

	// Update camera description
	QTextStream camera_desc(new QString());
	camera_desc << "<strong>" << current_camera_info.description() << "</strong><br>";
	camera_desc << "<em>Index:</em>    " << comboBox_camera->currentIndex() << "<br>";
	camera_desc << "<em>ID:</em>       " << current_camera_info.deviceName() << "<br>";
	camera_desc << "<em>Position:</em> " <<
				   cameraPositionStr(current_camera_info.position()) << "<br>";
	camera_desc << "<em>Focus:</em>    " << " to " << "<br>";
	camera_desc << "<em>Focus control:</em> " << isFocusAvailable << "<br>";
	camera_desc << "<em>Exposure control:</em> " << isExposureAvailable << "<br>";
	ui->label_camera_info->setText( *(camera_desc.string()) );
}

void MainWindow::fetchSaveDir()
{
	QString dir = QFileDialog::getExistingDirectory(this);
	ui->lineEdit_save_directory->setText(dir);
}

QString MainWindow::cameraPositionStr(QCamera::Position position)
{
	switch (position) {
		case QCamera::UnspecifiedPosition :
			return "unspecified";
		case QCamera::BackFace :
			return "back-facing";
		case QCamera::FrontFace :
			return "front-facing";
		default :
			return "undefined";
	}
}
