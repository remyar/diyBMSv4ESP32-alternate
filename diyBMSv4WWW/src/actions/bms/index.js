import getMonitor2 from "./get.monitor2";
import getMonitor3 from "./get.monitor3";
import getModule from "./get.modules";
import getStorage from "./get.storage";
import getSettings from "./get.settings";
import setGlobalSettings from "./set.globalSettings";
import setBankSettings from "./set.bankSettings";
import setModuleSettings from "./set.moduleSettings";
import setCounter from "./set.counter";
import restartController from "./restartController";
import identifyModule from "./identifyModule";
import setNtpSettings from "./set.ntpSettings";
import getRules from "./get.rules";
import setRules from "./set.rules";
import setStorage from "./set.storage";
import setSdUnmount from "./set.sd.unmount";
import setSdMount from "./set.sd.mount";
import setSaveWifi from "./set.save.wifi";
import setSaveConfig from "./set.save.config";
import setDisplaySettings from './set.display.settings';

export default {
    getMonitor2,
    getMonitor3,
    getSettings,
    getModule,
    getStorage,
    setGlobalSettings,
    setBankSettings,
    setCounter,
    restartController,
    identifyModule,
    setModuleSettings,
    setNtpSettings,
    getRules,
    setRules,
    setStorage,
    setSdUnmount,
    setSdMount,
    setSaveWifi,
    setSaveConfig,
    setDisplaySettings
}