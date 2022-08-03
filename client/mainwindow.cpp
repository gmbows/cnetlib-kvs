#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <cnet_utility.h>

#include <json.hpp>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	this->key_line = this->findChild<QLineEdit*>("keyLine");
	this->value_line = this->findChild<QLineEdit*>("valueLine");

	this->client.set_connection_handler([this](CN::Connection* cn) {
		QVariant data;
		data.setValue(cn);
		this->ui->connections->insertItem(0,QString::fromUtf8(std::to_string(cn->id)+": "+cn->address),data);
		this->ui->connections->setCurrentIndex(0);
	});

	this->client.set_msg_handler([this](CN::Message* msg) {
		switch(msg->type) {
			case (int)CN::DataType::TEXT: {
				std::string query_resp = CNetLib::fmt_bytes(msg->content.data(),msg->size);
				this->ui->queryResponse->setText(QString::fromStdString(query_resp));
				break;
			}
		}
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_connectButton_clicked() {
	std::string addr = this->ui->address->text().toStdString();
	if(addr.size() == 0) addr = "127.0.0.1";
	this->client.connect(addr);
	this->ui->address->clear();
}

void MainWindow::on_sendMessageButton_clicked() {
	std::string key = this->key_line->text().toStdString();
	std::string value = this->value_line->text().toStdString();
	if(key.empty() or value.empty()) return;
	nlohmann::json j = {{key,value}};
	QVariant data = this->ui->connections->currentData();
	if(data.isValid() == false) {
		return;
	}
	CN::Connection *cn = data.value<CN::Connection*>();
	cn->package_and_send(CN::DataType::JSON,j.dump());
	this->key_line->clear();
	this->value_line->clear();
}


void MainWindow::on_connections_currentIndexChanged(int index) {
	QVariant data = this->ui->connections->currentData();
	if(data.isValid() == false) {
		this->ui->sendMessageButton->setEnabled(false);
		this->ui->keyLine->setEnabled(false);
		this->ui->valueLine->setEnabled(false);
		this->ui->queryButton->setEnabled(false);
		this->ui->queryKey->setEnabled(false);
	} else {
		this->ui->sendMessageButton->setEnabled(true);
		this->ui->keyLine->setEnabled(true);
		this->ui->valueLine->setEnabled(true);
		this->ui->queryButton->setEnabled(true);
		this->ui->queryKey->setEnabled(true);
	}
}


void MainWindow::on_queryButton_clicked() {
	if(this->get_query_key().size() == 0) return;
	QVariant data = this->ui->connections->currentData();
	if(data.isValid() == true) {
		CN::Connection *cn = data.value<CN::Connection*>();
		cn->package_and_send(CN::DataType::TEXT,this->get_query_key());
	} else {

	}
}

std::string MainWindow::get_query_key() {
	if(this->ui->queryKey->text().size() > 0) {
		return this->ui->queryKey->text().toStdString();
	}
	return "";
}
