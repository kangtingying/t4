# MRQ升级工具
* 作者：丁建伟
* 对MRQ进行升级：
  * ARM
  * BOOT
  * FPGA
  
# 使用方法
* megarobo_update.exe <MRQ文件> <设备地址描述> <MRQ 接收ID>
* 例如 megarobo_update.exe xxx.upd tcpip::192.168.1.2::instr receiveid

# 更新记录

## 1.0.0.1
  * 根据MRQ对于升级流程的更新，在ARM写入完成后主动写入CA 16并等待回复
