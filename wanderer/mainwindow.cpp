#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
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
}

MainWindow::~MainWindow()
{
	delete ui;
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
