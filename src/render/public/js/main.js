const { ipcRenderer } = require('electron');

new Vue({
  created() {
    ipcRenderer.on('dispatch-DZC-res', (event, data) => {
      console.log(data);
    });
  },
  data: () => ({

  }),
  methods: {
    dispatchDZC() { // 下发电子称
      ipcRenderer.invoke('dispatch-DZC');
    },
  },
}).$mount('#root');
