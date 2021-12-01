import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import routeMdw from '../../middleware/route';

function NavBar(props) {
    return <div className="header">
        <div className="logocontainer">
            <img className="logo" src="img/logo.png" width="152" height="48" alt="DIYBMS" />
        </div>
        <div className="header-right">

            <div className="notify-container"><a className={props.navigation.getPath() == routeMdw.urlIndex() ? "active" : ""} style={{ cursor: "pointer" }} onClick={() => {
                props.navigation.push(routeMdw.urlIndex());
            }}>Home</a></div>

            <div className="notify-container"><a className={props.navigation.getPath() == routeMdw.urlModules() ? "active" : ""} style={{ cursor: "pointer" }} onClick={() => {
                props.navigation.push(routeMdw.urlModules());
            }}>Modules</a></div>

            <div className="notify-container"><a className={props.navigation.getPath() == routeMdw.urlSettings() ? "active" : ""} style={{ cursor: "pointer" }} onClick={() => {
                props.navigation.push(routeMdw.urlSettings());
            }}>Settings</a></div>

            <div className="notify-container" >
                <span className="notify-bubble" id="activerules"></span>
                <a id="rules" style={{ cursor: "pointer" }} onClick={() => {
                    props.navigation.push(routeMdw.urlRules());
                }}>Rules</a>
            </div>

            <div className="notify-container"><a id="more" href="#">More</a></div>
        </div>
    </div >
}

export default withStoreProvider(withNavigation(injectIntl(NavBar)));