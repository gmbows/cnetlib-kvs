#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLineEdit>
#include <QMainWindow>
#include <cnetlib.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	CN::Client client = CN::Client(5555);

	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_connectButton_clicked();

	void on_sendMessageButton_clicked();

	void on_connections_currentIndexChanged(int index);

	void on_queryButton_clicked();

private:

	QLineEdit* key_line;
	QLineEdit* value_line;
	std::string get_query_key();

	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
