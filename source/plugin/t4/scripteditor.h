#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QWidget>
#include "../plugin/xpage.h"
namespace Ui {
class ScriptEditor;
}

namespace mrx_t4 {

class ScriptEditor : public XPage
{
    Q_OBJECT

public:
    explicit ScriptEditor(QWidget *parent = 0);
    ~ScriptEditor();

private:
    Ui::ScriptEditor *ui;
};

}
#endif // SCRIPTEDITOR_H
