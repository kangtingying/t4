#include "t4para.h"
#include "../../include/mydef.h"

T4Para::T4Para()
{
    init();

    rst();
}

void T4Para::init()
{
    //! rst

    for ( int i = 0; i < T4Para::_axis_cnt; i++ )
    {
        mAxisZero[i] = 0;
    }

    mSlowMult = 25;
    mSlowDiv = 1;
}

//! keep the axis zero
void T4Para::rst()
{
    mTerminalType = T4Para::e_terminal_f2;
    mA5Range = T4Para::e_range_360;

    mAxisCurrents[0] = 3.5;
    mAxisCurrents[1] = 3.5;
    mAxisCurrents[2] = 3.5;
    mAxisCurrents[3] = 1.8;
    mAxisCurrents[4] = 1.5;

    mAxisIdleCurrents[0] = 1;
    mAxisIdleCurrents[1] = 1;
    mAxisIdleCurrents[2] = 1;
    mAxisIdleCurrents[3] = 1;
    mAxisIdleCurrents[4] = 0.5;

    //! \note do not change the zero
    mbAxisSoftEnable = false;
    mbAxisSafeEnable = true;
    for ( int i = 0; i < T4Para::_axis_cnt; i++ )
    {
        mAxisSoftUpper[ i ] = 180;
        mAxisSoftLower[ i ] = -180;

        mAxisSafeUpper[ i ] = 180;
        mAxisSafeLower[ i ] = -180;
    }

    mStepIndex = 5;
    mSpeed = 20;
    mCoord = e_coord_base;

//    mJointStepIndex = 6;
//    mJointSpeed = 0.2;

    for ( int i =0; i < sizeof_array(mCoordPara); i++ )
    {
        mCoordPara[i].mPx = 250;
        mCoordPara[i].mPy = 0;
        mCoordPara[i].mPz = 518.8;

        mCoordPara[i].mRa = 0;
        mCoordPara[i].mRb = 0;
        mCoordPara[i].mRc = 0;
    }

    mArmLength[0] = 263.7;
    mArmLength[1] = 255;
    mArmLength[2] = 250;

    mHomeSpeed = 20;
    mHomeTimeout = 120;

    mMaxAcc = 10;
    mMaxJerk = 10;
    mAutoAcc = 25;

    mMaxTerminalSpeed = 250;

    mMaxJointSpeeds.clear();
    mMaxJointSpeeds<<170<<50<<50<<60<<60;

    mbAxisPwr = true;
    mbMctEn = true;

    //! \note the delta angle for
    mPackagesAxes[0] = 0;
    mPackagesAxes[1] = +18.8;
    mPackagesAxes[2] = -57.4;
    mPackagesAxes[3] = -103;

    //! digital inputs name
    if( listIoName.size() !=0 )
        listIoName.clear();
    listIoName << QString("XIN1") << QString("XIN2") << QString("XIN3") << QString("XIN4")
               << QString("XIN5") << QString("XIN6") << QString("XIN7") << QString("XIN8")
               << QString("XIN9") << QString("XIN10")
               << QString("Y1") << QString("Y2") << QString("Y3") << QString("Y4");
}

double T4Para::velocity()
{
    return mMaxTerminalSpeed * mSpeed / 100;
}

int T4Para::serialOut( QXmlStreamWriter &writer )
{
    writer.writeStartElement("terminal");
        writer.writeTextElement( "type", QString::number( (int)mTerminalType ) );
        writer.writeTextElement( "a5_range", QString::number( (int)mA5Range ) );
    writer.writeEndElement();

    writer.writeStartElement( "safe_limit" );
        writer.writeTextElement( "soft_limit", QString::number( mbAxisSoftEnable) );
        writer.writeTextElement( "safe_area", QString::number( mbAxisSafeEnable) );
    writer.writeEndElement();

    for ( int i = 0; i < T4Para::_axis_cnt; i++ )
    {
        writer.writeStartElement("axis");
            writer.writeTextElement( "current", QString::number( mAxisCurrents[i]) );
            writer.writeTextElement( "idle_current", QString::number( mAxisIdleCurrents[i]) );

            writer.writeTextElement( "zero", QString::number( mAxisZero[i]) );

            writer.writeTextElement( "upper", QString::number( mAxisSoftUpper[i]) );
            writer.writeTextElement( "lower", QString::number( mAxisSoftLower[i]) );
        writer.writeEndElement();
    }

    //! coord
    writer.writeStartElement("coord");

    writer.writeTextElement( "type", QString::number( (int)mCoord ) );

    for ( int i = 0; i < sizeof_array(mCoordPara); i++ )
    {
        writer.writeStartElement("para");

        writer.writeTextElement( "px", QString::number( mCoordPara[i].mPx ) );
        writer.writeTextElement( "py", QString::number( mCoordPara[i].mPy ) );
        writer.writeTextElement( "pz", QString::number( mCoordPara[i].mPz ) );

        writer.writeTextElement( "ra", QString::number( mCoordPara[i].mRa ) );
        writer.writeTextElement( "rb", QString::number( mCoordPara[i].mRb ) );
        writer.writeTextElement( "rc", QString::number( mCoordPara[i].mRc ) );

        writer.writeEndElement();
    }

    writer.writeEndElement();

    //! arm
    writer.writeStartElement("arm");
        writer.writeTextElement( "base", QString::number( mArmLength[0] ) );
        writer.writeTextElement( "big_arm", QString::number( mArmLength[1] ) );
        writer.writeTextElement( "little_arm", QString::number( mArmLength[2] ) );
    writer.writeEndElement();

    //! robo speed
    writer.writeStartElement("speed");
        writer.writeTextElement( "percent", QString::number( mSpeed ) );
        writer.writeTextElement( "step", QString::number( mStepIndex ) );
    writer.writeEndElement();

//    //! joint speed
//    writer.writeStartElement("joint_speed");
//        writer.writeTextElement( "percent", QString::number( mJointStepIndex ) );
//        writer.writeTextElement( "step", QString::number( mJointStepIndex ) );
//    writer.writeEndElement();

    //! home
    writer.writeStartElement("home");
        writer.writeTextElement( "speed", QString::number( mHomeSpeed ) );
        writer.writeTextElement( "timeout", QString::number( mHomeTimeout ) );
    writer.writeEndElement();

    //! speed
    writer.writeStartElement("max_speed");
        writer.writeTextElement( "acc", QString::number( mMaxAcc ) );
        writer.writeTextElement( "jerk", QString::number( mMaxJerk ) );
        writer.writeTextElement( "auto_acc", QString::number( mAutoAcc ) );

        writer.writeTextElement( "terminal", QString::number( mMaxTerminalSpeed ) );

        //! each joint
        for ( int i = 0; i < mMaxJointSpeeds.size(); i++ )
        {
            writer.writeTextElement( "joint", QString::number( mMaxJointSpeeds.at(i) ) );
        }

    writer.writeEndElement();

    //! control
    writer.writeStartElement("control");
        writer.writeTextElement( "driver_enable", QString::number( mbAxisPwr ) );
        writer.writeTextElement( "mct_enable", QString::number( mbMctEn ) );
    writer.writeEndElement();

    //! slow
    writer.writeStartElement("slow");
        writer.writeTextElement( "mult", QString::number( mSlowMult ) );
        writer.writeTextElement( "div", QString::number( mSlowDiv ) );
    writer.writeEndElement();

    //! digital inputs
    writer.writeStartElement("digital_inputs");
        writer.writeTextElement("name", QString(listIoName.join(",")));
    writer.writeEndElement();

    return 0;
}
int T4Para::serialIn( QXmlStreamReader &reader )
{
    int axisId = 0;
    while( reader.readNextStartElement() )
    {
        if ( reader.name() == "terminal" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "type" )
                {
                    mTerminalType = (eTerminalType)reader.readElementText().toInt();
                }
                else if ( reader.name() == "a5_range" )
                {
                    mA5Range = (eAxis5Range)reader.readElementText().toInt();
                }
                else
                { reader.skipCurrentElement(); }
            }
        }

        else if ( reader.name() == "safe_limit" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "soft_limit" )
                { mbAxisSoftEnable = reader.readElementText().toInt() > 0; }
                else if ( reader.name() == "safe_area" )
                { mbAxisSafeEnable = reader.readElementText().toInt() > 0; }
                else
                { reader.skipCurrentElement(); }
            }
        }

        else if ( reader.name() == "axis" )
        {
            axisId++;
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "current" )
                {
                    Q_ASSERT( axisId <= _axis_cnt );
                    mAxisCurrents[ axisId - 1] = reader.readElementText().toDouble();
                }

                else if ( reader.name() == "idle_current" )
                {
                    Q_ASSERT( axisId <= _axis_cnt );
                    mAxisIdleCurrents[ axisId - 1] = reader.readElementText().toDouble();
                }

//                else if ( reader.name() == "switch_time" )
//                {
//                    Q_ASSERT( axisId <= _axis_cnt );
//                    mAxisSwitchTimes[ axisId - 1] = reader.readElementText().toDouble();
//                }

                else if ( reader.name() == "zero" )
                {
                    Q_ASSERT( axisId <= _axis_cnt );
                    mAxisZero[ axisId - 1] = reader.readElementText().toDouble();
                }
                else if ( reader.name() == "upper" )
                {
                    Q_ASSERT( axisId <= _axis_cnt );
                    mAxisSoftUpper[ axisId - 1] = reader.readElementText().toDouble();
                }
                else if ( reader.name() == "lower" )
                {
                    Q_ASSERT( axisId <= _axis_cnt );
                    mAxisSoftLower[ axisId - 1] = reader.readElementText().toDouble();
                }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else if ( reader.name() == "coord" )
        {
            int id = -1;
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "type" )
                {
                    mCoord = (eCoordinateType)reader.readElementText().toInt();
                }
                else if ( reader.name() == "para" )
                {
                    id++;
                    while( reader.readNextStartElement() )
                    {
                        if ( reader.name() == "px" )
                        { mCoordPara[id].mPx = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "py")
                        { mCoordPara[id].mPy = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "pz")
                        { mCoordPara[id].mPz = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "ra")
                        { mCoordPara[id].mRa = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "rb")
                        { mCoordPara[id].mRb = reader.readElementText().toDouble(); }
                        else if ( reader.name() == "rc")
                        { mCoordPara[id].mRc = reader.readElementText().toDouble(); }
                        else
                        { reader.skipCurrentElement(); }
                    }
                }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else if ( reader.name() == "arm" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "base" )
                { mArmLength[0] = reader.readElementText().toDouble(); }
                else if ( reader.name() == "big_arm" )
                { mArmLength[1] = reader.readElementText().toDouble(); }
                else if ( reader.name() == "little_arm" )
                { mArmLength[2] = reader.readElementText().toDouble(); }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else if ( reader.name() == "speed" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "percent" )
                { mSpeed = reader.readElementText().toDouble(); }
                else if ( reader.name() == "step" )
                { mStepIndex = reader.readElementText().toInt(); }
                else
                { reader.skipCurrentElement(); }
            }
        }
//        else if ( reader.name() == "joint_speed" )
//        {
//            while( reader.readNextStartElement() )
//            {
//                if ( reader.name() == "percent" )
//                { mJointSpeed = reader.readElementText().toDouble(); }
//                else if ( reader.name() == "step" )
//                { mJointStepIndex = reader.readElementText().toInt(); }
//                else
//                { reader.skipCurrentElement(); }
//            }
//        }
        else if ( reader.name() == "home" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "speed" )
                { mHomeSpeed = reader.readElementText().toDouble(); }
                else if ( reader.name() == "timeout" )
                { mHomeTimeout = reader.readElementText().toInt(); }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else if ( reader.name() == "max_speed" )
        {
            int jid = 0;
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "acc" )
                { mMaxAcc = reader.readElementText().toDouble(); }
                else if ( reader.name() == "jerk" )
                { mMaxJerk = reader.readElementText().toDouble(); }
                else if ( reader.name() == "auto_acc" )
                { mAutoAcc = reader.readElementText().toInt(); }
                else if ( reader.name() == "joint" )
                {
                    mMaxJointSpeeds[jid++] = reader.readElementText().toDouble();
                }
                else if ( reader.name() == "terminal" )
                { mMaxTerminalSpeed = reader.readElementText().toInt(); }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else if ( reader.name() == "control" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "driver_enable" )
                { mbAxisPwr = reader.readElementText().toInt() > 0; }
                else if ( reader.name() == "mct_enable" )
                { mbMctEn = reader.readElementText().toInt() > 0; }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else if ( reader.name() == "slow" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "mult" )
                { mSlowMult = reader.readElementText().toInt(); }
                else if ( reader.name() == "div" )
                { mSlowDiv = reader.readElementText().toInt(); }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else if ( reader.name() == "digital_inputs" )
        {
            while( reader.readNextStartElement() )
            {
                if ( reader.name() == "name" )
                {
                    listIoName.clear();
                    listIoName = reader.readElementText().split( "," );
                }
                else
                { reader.skipCurrentElement(); }
            }
        }
        else
        {
            reader.skipCurrentElement();
        }
    }

    return 0;
}
