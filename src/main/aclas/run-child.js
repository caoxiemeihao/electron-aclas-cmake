const path = require('path');
const aclas = require('../../../build/Debug/aclas.node');

const config = {
  host: '192.168.1.2',
  type: 0x0000,
  filename: path.join(__dirname, 'test/plu.txt'),
  dll: path.join(__dirname, 'Win64/AclasSDK.dll'), // md5: 35b248a0c1c35c39e90d2f17408b6ea4
};

aclas(config, () => {});
