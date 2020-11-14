const path = require('path');
const aclas = require('../../../build/Debug/aclas.node');
// const aclas = require('../../../build/Release/aclas.node');

exports.code_dict = {
  256: '已初始化',
  257: '未初始化',
  258: '设备不存在',
  259: '不支持的协议类型',
  260: '该数据类型不支持此操作',
  261: '该数据类型不支持',
  264: '无法打开输入文件',
  265: '字段数与内容数不匹配',
  266: '通讯数据异常',
  267: '解析数据异常',
  268: 'CodePage错误',
  269: '无法创建输出文件',
  0: 'sucessed',
  1: 'processing',
  // 自定义错误
  401: '报错 [写入 plu.txt 失败]',
  403: '报错 [链接超时]',
  404: '报错 [加载 AclasSDK.dll 失败]',
  405: '报错 [polyfill addons.]',
  406: '报错 [子进程拉起多次失败]',
  407: '报错 [C# addons 子进程]',
  408: '报错 [C# addons 正在运行]',
  409: '报错 [trycatch 未知异常]',
  431: '报错 [with-csharp stderr]',
  441: '报错 [with-cc stderr]',
  501: 'ping [超时]',
  502: 'ping [不通]'
};

const config = {
  host: '192.168.1.2',
  type: 0x0000,
  filename: path.join(__dirname, 'test/plu.txt'),
  dll: path.join(__dirname, 'Win64/AclasSDK.dll'), // md5: 35b248a0c1c35c39e90d2f17408b6ea4
};

aclas(config, () => {});
