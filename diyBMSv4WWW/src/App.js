import React, { useState, useEffect } from 'react';

import NavBar from "./components/navbar";
import Infos from "./components/infos";

import { withStoreProvider } from './providers/StoreProvider';

import { Route } from 'react-router-dom';

import routeMdw from './middleware/route';

import HomePage from './pages/home';
import ModulesPage from './pages/modules';
import SettingsPage from './pages/settings';
import RulesPage from './pages/rules';
import AboutPage from './pages/about';
import StoragePage from './pages/storage';

import actionsBms from './actions/bms';

const routes = [
    { path: routeMdw.urlIndex(), name: 'homePage', Component: HomePage },
    { path: routeMdw.urlModules(), name: 'modulesPage', Component: ModulesPage },
    { path: routeMdw.urlSettings(), name: 'settingsPage', Component: SettingsPage },
    { path: routeMdw.urlRules(), name: 'rulesPage', Component: RulesPage },
    { path: routeMdw.urlAbout(), name: 'aboutPage', Component: AboutPage },
    { path: routeMdw.urlStorage(), name: 'storagePage', Component: StoragePage },
]

function App(props) {

    const monitor2 = props.globalState.monitor2 || {};

    useEffect(() => {

        props.dispatch(actionsBms.getRules());
        props.dispatch(actionsBms.getSettings());

        props.dispatch(actionsBms.getMonitor2());
        props.dispatch(actionsBms.getMonitor3());

        props.dispatch(actionsBms.getStorage());

        let d = 0;
        const interval = setInterval(() => {
            props.dispatch(actionsBms.getSettings());
            props.dispatch(actionsBms.getMonitor2());
            props.dispatch(actionsBms.getMonitor3());
            props.dispatch(actionsBms.getRules());
            d++;
            if ((d % 6) == 0) {
                props.dispatch(actionsBms.getStorage());
            }
        }, 5000);

        return () => clearInterval(interval);
    }, []);

    let infosTab = [];

    if (monitor2.bankv) {
        for (var bankNumber = 0; bankNumber < monitor2.bankv.length; bankNumber++) {
            infosTab.push(<div id={"voltage" + bankNumber} className="stat"><span className="x t">Voltage {bankNumber} :</span><span class="x v">{(parseFloat(monitor2.bankv[bankNumber]) / 1000.0).toFixed(2) + "V"}</span></div>);
            infosTab.push(<div id={"range" + bankNumber} className="stat"><span className="x t">Range {bankNumber} :</span><span class="x v">{monitor2.voltrange[bankNumber] + "mV"}</span></div>);
        }
    }

    return <div>
        <NavBar />
        <Infos infosTab={infosTab} />
        <div className="page">
            {routes.map(({ path, Component }) => (
                <Route path={path} key={path} exact component={Component} />
            ))}
        </div>
    </div>;
}

export default withStoreProvider(App);
