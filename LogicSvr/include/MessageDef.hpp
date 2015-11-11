#ifndef __MESSAGEDEF_H__
#define __MESSAGEDEF_H__
enum eServerMessageClass //server message type
{
    eRegister_Message = 1,
    eServerMessage_Client = 2,
    eServerMessage_AccSvr = 3,
    eServerMessage_GateWay = 4,
    eServerMessage_Logic = 5,
    eServerMessage_DBServer = 6,
};


enum eServerMessage_Client_MsgType // msg from client 2
{
    erverMessage_Client_MsgType_begin,
    CLI_ACCS_TESTBINPKG,  // JUST TEST
    CLI_ACCS_TESTPROBUFPKG, // JUST TEST
    CLI_ACCS_CHECKLOGINUSER,
    erverMessage_Client_MsgType_end,
};

enum eServerMessage_AccSvr_MsgType // msg from accSvr 3
{
    eServerMessage_AccSvr_MsgType_begin,
    ACCS_DBS_CHECKLOGINUSER,
    ACCS_DBS_TEST, //-----JUST TEST HANDLER

    ACCS_CLI_CHECKLOGINUSER_RET,
    eServerMessage_AccSvr_MsgType_end,
};

enum eServerMessage_GateWay_MsgType // msg from gateway 4
{
    eServerMessage_GateWay_MsgType_begin,
    eServerMessage_GateWay_MsgType_end,
};

enum eServerMessage_Logic_MsgType // msg from logic svr 5
{
    eServerMessage_Logic_MsgType_begin,
    eServerMessage_Logic_MsgType_end,
};

enum eServerMessage_DBServer_MsgType // msg from dbserver 6
{
    eServerMessage_DBServer_MsgType_begin,
    DBS_ACCS_CHECKLOGINUSER_RET = 2,
    eServerMessage_DBServer_MsgType_end,
};

#endif

