const fs = require('fs')
const path = require('path')

/**
 * log地址：C:\Users\用户\AppData\Local\Temp\ypshop
 * @param {string} data 
 * @param {string} filename 
 */
exports.log = function log({ data = '', filename = 'log.log', append = true }) {
  return new Promise(resolve => {
    const ypshop = path.join(os.tmpdir(), 'ypshop')
    fs.existsSync(ypshop) || fs.mkdirSync(ypshop)
    const _data = new Date().toLocaleString() + '\n' + data + '\n'
    fs[append ? 'appendFile' : 'writeFile'](path.join(ypshop, filename), _data, resolve)
  })
}

/**
 * C++ std::cout | C# Console.WriteLine() 解析
 * 格式: ##数据##
 * @param {string} str 
 */
exports.parse_str2arr = function parse_str2arr(str) {
  let result = []
  // stdout 有两次 Console.WriteLine() 合并的情况，所以 res 是数组
  let res = String(str).trim().match(/(##(\w+)=(\{[\s\S][^##]+\})##)/g)
  if (Array.isArray(res)) {
    res.forEach(r => {
      const tmp = r.match(/^##(\w+)=(\{[\s\S]+\})##$/)
      if (Array.isArray(tmp)) {
        const cmd = tmp[1]
        let json = tmp[2]
        try {
          json = JSON.parse(json.replace(/\n/g, '<br/>'))
        } catch (error) {
          console.log()
          console.log((json))
          console.log('----')
          console.log(error)
          console.log()
          json = {}
        }
        result.push({ cmd, json })
      }
    })
  }

  return result
}
