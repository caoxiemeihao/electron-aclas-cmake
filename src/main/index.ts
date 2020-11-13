import { app, BrowserWindow, ipcMain } from 'electron'
import { port } from '../../env'
import aclas from './aclas'

let win: BrowserWindow

function bootstrap() {
  win = new BrowserWindow({
    webPreferences: {
      nodeIntegration: true,
    },
  })

  ipcHandle()

  win.loadURL(`http://127.0.0.1:${port}`)
}

function ipcHandle() {
  ipcMain.handle('dispatch-DZC', (event, arg0 = {}) => {
    aclas(arg0, data => {
      event.sender.send('dispatch-DZC-res', data)
    })
  })
}

app.whenReady().then(bootstrap)
app.on('window-all-closed', () => { win = null })
