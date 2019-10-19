#include "testdata_display.h"
#include "ui_testdata_display.h"


UINT8 test_data[20];




TestData_Display::TestData_Display(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TestData_Display)
{
  ui->setupUi(this);
  data_fresh();
}

TestData_Display::~TestData_Display()
{
  delete ui;
}


void TestData_Display::data_fresh()
{
 ui->tl_0->setText( QString("%1").arg(test_data[0],8, 2,QChar('0') ) );
  ui->tl_1->setText( QString("%1").arg(test_data[1],8, 2,QChar('0') ) );
  ui->tl_2->setText( QString("%1").arg(test_data[2],8, 2,QChar('0') ) );
  ui->tl_3->setText( QString("%1").arg(test_data[3],8, 2,QChar('0') ) );
  ui->tl_4->setText( QString("%1").arg(test_data[4],8, 2,QChar('0') ) );
  ui->tl_5->setText( QString("%1").arg(test_data[5],8, 2,QChar('0') ) );
  ui->tl_6->setText( QString("%1").arg(test_data[6],8, 2,QChar('0') ) );
  ui->tl_7->setText( QString("%1").arg(test_data[7],8, 2,QChar('0') ) );
  ui->tl_8->setText( QString("%1").arg(test_data[8],8, 2,QChar('0') ) );
  ui->tl_9->setText( QString("%1").arg(test_data[9],8, 2,QChar('0') ) );

  ui->tl_10->setText( QString("%1").arg(test_data[10],8, 2,QChar('0') ) );
  ui->tl_11->setText( QString("%1").arg(test_data[11],8, 2,QChar('0') ) );
  ui->tl_12->setText( QString("%1").arg(test_data[12],8, 2,QChar('0') ) );
  ui->tl_13->setText( QString("%1").arg(test_data[13],8, 2,QChar('0') ) );
  ui->tl_14->setText( QString("%1").arg(test_data[14],8, 2,QChar('0') ) );
  ui->tl_15->setText( QString("%1").arg(test_data[15],8, 2,QChar('0') ) );
  ui->tl_16->setText( QString("%1").arg(test_data[16],8, 2,QChar('0') ) );
  ui->tl_17->setText( QString("%1").arg(test_data[17],8, 2,QChar('0') ) );
  ui->tl_18->setText( QString("%1").arg(test_data[18],8, 2,QChar('0') ) );
  ui->tl_19->setText( QString("%1").arg(test_data[19],8, 2,QChar('0') ) );


}
