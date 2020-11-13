import { join } from 'path'

const _config = {
  host: '192.168.1.2',
  filename: join(__dirname, './test/plu.txt'),
  dll: join(__dirname, './Win64/AclasSDK.dll'),
  type: 0x0000,
}

function runAclas(config, callback) {
  const conf = Object.assign(_config, config)

  callback(conf)
}

export default runAclas
