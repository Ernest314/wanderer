#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QString>
#include <QTextStream>

#include <QCamera>
#include <QCameraInfo>
#include <QCameraFocus>
#include <QCameraExposure>
#include <QCameraImageCapture>
#include <QMediaRecorder>
#include <QCameraImageProcessing>

#include <QComboBox>
#include <QCameraViewfinder>

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private slots:
	void updateCameraCaptureSettings();

private:
	Ui::MainWindow* ui;

	QList<QCameraInfo> cameras;
	QCamera* camera;
	QComboBox* comboBox_camera;

	QString cameraPositionStr(QCamera::Position position);
};

#endif // MAINWINDOW_H
