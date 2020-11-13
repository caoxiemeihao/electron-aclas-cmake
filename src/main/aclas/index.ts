import { join } from 'path'
import { fork } from 'child_process'

// const _config = {
//   host: '192.168.1.2',
//   filename: join(__dirname, './test/plu.txt'),
//   dll: join(__dirname, './Win64/AclasSDK.dll'),
//   type: 0x0000,
// }

function runAclas(config, callback) {
  // const conf = Object.assign(_config, config)

  const handle = fork(join(__dirname, 'aclas/run-child.js'), { stdio: 'pipe', })
  handle.stdout.on('data', chunk => {
    const str = chunk.toString()
    console.log(str)
  })
  handle.stderr.on('data', chunk => {
    const str = chunk.toString()
    console.log('ERROR >>\n', str)
  })
}

export default runAclas
