#include "changeinfo.h"
#include "ui_changeinfo.h"
#include "membersearch.h"
#include <QTableView>
ChangeInfo::ChangeInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangeInfo)
{
    ui->setupUi(this);
    Tree* tr,tr1;
    tr=new Tree;
    tr1=new Tree;
    ui->changeinfoview->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //ui->inputline->setPlaceholderText("请输入姓名");
        //判断是否保存
        judge=1;
        model=new QSqlTableModel(this);

        model->setTable("member");
        model->select();
        model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        model->setHeaderData(0,Qt::Horizontal,"name");
        model->setHeaderData(1,Qt::Horizontal,"gender");
        model->setHeaderData(2,Qt::Horizontal,"fathername");
        model->setHeaderData(3,Qt::Horizontal,"islive");
        model->setHeaderData(4,Qt::Horizontal,"address");
        model->setHeaderData(5,Qt::Horizontal,"telnumber");
        model->setHeaderData(6,Qt::Horizontal,"generation");
        model->removeColumns(7,7);
        _model=new QSqlTableModel(this);
        _model->setTable("member");
        _model->setEditStrategy(QSqlTableModel::OnManualSubmit);
        _model->setHeaderData(0,Qt::Horizontal,"name");
        _model->setHeaderData(1,Qt::Horizontal,"gender");
        _model->setHeaderData(2,Qt::Horizontal,"fathername");
        _model->setHeaderData(3,Qt::Horizontal,"islive");
        _model->setHeaderData(4,Qt::Horizontal,"address");
        _model->setHeaderData(5,Qt::Horizontal,"telnumber");
        _model->setHeaderData(6,Qt::Horizontal,"generation");
        _model->removeColumns(7,7);

        //两个按钮信号和槽函数连接
        connect(ui->confirmbtn,SIGNAL(clicked()),this,SLOT(confirmbtnSlot()));
        connect(ui->returnbtn,SIGNAL(clicked()),this,SLOT(returnbtnSlot())) ;
        //connect(ui->inputline,SIGNAL(textChanged(QString)),this,SLOT(inputeditSlot(QString)));
        connect(model,SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(judgeSlot(QModelIndex,QModelIndex)));

        QTableView *view = new QTableView;
        view->setModel(model);
        view->show();
        int i;
        i=view->currentIndex().row();
        QString name=model.record(i).value("name").toString();
        tr1=search(tr,name);
}

ChangeInfo::~ChangeInfo()
{
    delete ui;
}
void ChangeInfo::inputeditSlot(QString)
{
    if(ui->inputline->text().isEmpty())
    {
        ui->teachersearchview->setModel(model);
    }
}
void ChangeInfo::comeFamilyManage(Tree* &tr)
{

    if(model->data(model->index(0,0)).toString().isEmpty())
    {
        QMessageBox::information(this,"提示","信息为空，请增加信息",QMessageBox::Yes);
        emit EmitToFamilyManageToChangeStack();
        this->hide();
        return;
    }
    ui->changeinfoview->setModel(model);
}
void TeacherChangeScore::confirmbtnSlot()
{
    if(model->submitAll())
    {
        QMessageBox::information(this,"提示","修改成功",QMessageBox::Yes);
        _model->setFilter( "name='"+name+"'");
        tr1->selfname=model.record(0).value("name").toString();
        tr1->gender=model.record(0).value("gender").toString();
        tr1->fathername=model.record(0).value("fathername").toString();
        tr1->telnum=model.record(0).value("telnumber").toString();
        tr1->address=model.record(0).value("address").toString();
        tr1->generation=model.record(0).value("name").toInt();
    }
    else
    {
        QMessageBox::information(this,"提示","修改失败",QMessageBox::Yes);
        model->revertAll();
    }
    judge=1;
}
void ChangeInfo::returnbtnSlot()
{

    if(judge==1)
    {
        emit EmitToFamilyManage();
        this->hide();
        return;
    }
    if(QMessageBox::question(this,"提示","尚未保存，是否保存",QMessageBox::Yes,QMessageBox::No)==QMessageBox::Yes)
    {
        if(model->submitAll())
            QMessageBox::information(this,"提示","修改成功",QMessageBox::Yes);
        else
        {
            QMessageBox::information(this,"提示","修改失败",QMessageBox::Yes);
            model->revertAll();
        }
        emit EmitToFamilyManage();
        this->hide();
        judge=1;
        return ;
    }
    else
    {
        emit EmitToFamilyManage();
        judge=1;
        this->hide();
    }
}
void ChangeInfo::judgeSlot(QModelIndex, QModelIndex)
{
    judge=0;
}
