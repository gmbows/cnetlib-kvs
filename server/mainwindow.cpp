#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "kvtable.h"
#include <json.hpp>
#include <fstream>
#include <map>

std::map<std::string,std::string> updated_map = {};

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow) {
	ui->setupUi(this);

	if(!CNetLib::file_exists(this->table_fname)) {
		std::ofstream of(table_fname);
		of << "{}";
		of.close();
	}


	this->table = this->findChild<QTableView*>("table");

	KVTable* model = new KVTable(this);
	this->table->setModel(model);

	this->update_table();

	this->server.set_msg_handler([this](CN::Message* msg) {
		switch(msg->type) {
			case (int)CN::DataType::JSON: {
				nlohmann::json received = nlohmann::json::parse(CNetLib::fmt_bytes(msg->content.data(),msg->size));
				this->append_to_data_file(received,msg->connection->address);
				this->update_table();
				break;
			}
			case (int)CN::DataType::TEXT: {
				std::string query_key = CNetLib::fmt_bytes(msg->content.data(),msg->size);
				std::ifstream f(table_fname);
				nlohmann::json current_data = nlohmann::json::parse(f);
				f.close();
				auto query_response = current_data[query_key];
				if(query_response.is_null()) query_response = "<null>";
				msg->connection->package_and_send(CN::DataType::TEXT,query_response);
				break;
			}
		}
	});

	this->server.set_connection_handler([this](CN::Connection* cn) {
		std::string cur_c = this->ui->connections->text().toStdString();
		this->ui->connections->setText(QString::fromStdString(std::to_string(std::stoi(cur_c)+1)));
	});

}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::update_table() {
	this->table->reset();

	std::ifstream f(table_fname);
	nlohmann::json data = nlohmann::json::parse(f);

	KVTable* model = new KVTable(this);
	model->table_data = data;
	model->updated_map = updated_map;
	this->table->setModel(model);
}

void MainWindow::append_to_data_file(nlohmann::json received,std::string address) {
	std::ifstream f(table_fname);
	nlohmann::json current_data = nlohmann::json::parse(f);
	f.close();

	//Append newly received data
	for(auto &item : received.items()) {
		current_data[item.key()] = item.value();
		if(updated_map.find(item.key()) != updated_map.end())
			updated_map.erase(item.key());
		updated_map.insert({item.key(),address});
	}
	std::ofstream o(table_fname);
	o << current_data;
	o.close();
}

void MainWindow::on_startServerButton_clicked() {
	this->ui->serverStatus->setText("Listening");
	this->server.start_listener();
}

void MainWindow::on_clearTableButton_clicked() {
	std::ofstream of(table_fname);
	of << "{}";
	of.close();
	this->update_table();
}
