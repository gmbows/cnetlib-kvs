#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <cnetlib.h>
#include <json.hpp>
#include <qtableview.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:

	CN::Server server = CN::Server(5555);

	void update_table();
	void append_to_data_file(nlohmann::json,std::string);
	std::string table_fname = "table.json";

	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_startServerButton_clicked();

	void on_clearTableButton_clicked();

private:
	QTableView *table;
	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
