#ifndef ERRMGRITEM_H
#define ERRMGRITEM_H

#include <QtCore>

enum e_event_type
{
    e_error = 1,
    e_warning,
    e_info
};


//enum e_event_action
//{
//    e_action_freewheel,
//    e_action_qs,
//    e_action_record_dec,
//    e_action_finish,
//};

#define ACTION_FREEWHEEL    "Free-wheeling"
#define ACTION_QSDEC        "QS deceleration"
#define ACTION_RECDEC       "Record deceleration"
#define ACTION_FINISH       "Finish Record"


//! bits
enum eColumnAttr
{
    column_unk = 0,
    column_editable = 0,
    column_empty = 1,
};



class ErrMgrItem
{
public:
    static int columns();
    static QString header( int col );

public:
    ErrMgrItem();

public:
    eColumnAttr columnAttr( int col );

    void setEventType( e_event_type evt, bool b );

public:
    bool mbErrorAble, mbWarnAble, mbInfoAble;
    bool mbOutputAble;
    bool mbSaveAble;
    bool mbActionAble;

    int mNr;
    QString mErr;
    e_event_type mEventType;
    QString mAction;
    QStringList mActionList;
    bool mbOutput;
    bool mbSaveDiagnosis;
};

#endif // ERRMGRITEM_H
