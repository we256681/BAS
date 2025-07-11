'use strict';

Vue.use(window['v-click-outside']);

const app = new Vue({
  i18n,
  store,
  render: h => h(App),
}).$mount('#app');
