#include "scripteditor.h"
#include "ui_scripteditor.h"

namespace mrx_t4 {

ScriptEditor::ScriptEditor(QWidget *parent) :
    XPage(parent),
    ui(new Ui::ScriptEditor)
{
    ui->setupUi(this);
}

ScriptEditor::~ScriptEditor()
{
    delete ui;
}

}
