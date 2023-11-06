#ifndef BABYLON_CONSTANTS_H_INCLUDED
#define BABYLON_CONSTANTS_H_INCLUDED



#endif // BABYLON_CONSTANTS_H_INCLUDED
 //CONSTATNTS
 const uint8_t CTRL_CMD = 69;
 const uint8_t DATA_CMD = 96;

 uint8_t CR_INIT_CMD_HEAD[4] = {'x','x','x','x'};
 uint8_t CT_INIT_CMD_HEAD[4] = {'x','X','X','x'};
 uint8_t INIT_CMD_TAIL[4] = {'x',22,22,'x'};
 uint8_t CR_INIT_CMD_FIN[4] = {'x',44,44,'x'};
 uint8_t CR_INIT_CMD_FIN_OK[4] = {'X',44,44,'x'};
 uint8_t CT_INIT_CMD_FIN[4] = {'X',44,44,'X'};

 uint8_t TRESET_HEAD[4] = {11,'x','x',10};
 uint8_t TRESET_TAIL[4] = {99,'x','x',90};

 uint8_t BACK_HEAD[4] = {19,'x','x',98};
 uint8_t BACK_TAIL[4] = {19,'x','x',99};

 uint8_t RRESET_HEAD[4] = {11,'x','x',11};
 uint8_t RRESET_TAIL[4] = {99,'x','x',99};

 const int MAX_RCOUNT = 50;

 const u_int DEFAULT_MTU = 1500;
 const u_int DEFAULT_MSS = DEFAULT_MTU - 40;
