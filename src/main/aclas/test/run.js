const path = require('path');
const aclas = require('../../../../build/Debug/aclas.node');

console.log(aclas);

const config = {
  host: '192.168.1.2',
  type: 0x0000,
  filename: path.join(__dirname, 'plu.txt'),
  dll: path.join(__dirname, '../Win64/AclasSDK.dll'), // md5: 35b248a0c1c35c39e90d2f17408b6ea4
};

aclas(config, () => {});
