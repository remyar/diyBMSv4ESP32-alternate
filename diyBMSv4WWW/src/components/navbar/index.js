import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import routeMdw from '../../middleware/route';

function NavBar(props) {

    const monitor2 = props.globalState.monitor2 || {};
    const [overlay, setOverlay] = useState(false);

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
                <span className="notify-bubble" id="activerules" style={{ display: monitor2.activerules > 0 ? "block" : "none" }}>{monitor2.activerules}</span>
                <a id="rules" style={{ cursor: "pointer" }} onClick={() => {
                    props.navigation.push(routeMdw.urlRules());
                }}>Rules</a>
            </div>

            <div style={{ cursor: 'pointer' }} className="notify-container"><a id="more" onClick={() => {
                setOverlay(true);
            }}>More</a></div>
        </div>


        <div id="myNav" className="overlay" style={{ height: overlay ? "100%" : "0%" }}>
            <a style={{ cursor: 'pointer' }} id="closebtn" className="closebtn" onClick={() => {
                setOverlay(false);
            }}>&times;</a>

            <div className="overlay-content" style={{ textAlign : "center"}}>
                <table style={{ marginLeft: "auto", marginRight: "auto" }}>
                   {/* <tr style={{ textAlign : "center"}}>
                        <td>
                            <a id="integration">Integration</a>
                        </td>
                    </tr>
                    <tr style={{ textAlign : "center"}}>
                        <td >
                            <a id="currentmonitor" >diyBMS Current Monitor</a>
                        </td>
                    </tr>
                    <tr>
                        <td>
                            <a id="victroncanbus" >Victron CANBUS BMS</a>
                        </td>
        </tr>*/}
                    <tr>
                        <td>
                            <a style={{ cursor: 'pointer' }} id="storage" onClick={()=>{
                                setOverlay(false);
                                props.navigation.push(routeMdw.urlStorage());
                            }}>Storage</a>
                        </td>
                    </tr>
                 {/*   <tr>
                        <td>
                            <a id="avrprogrammer" >AVR Programmer</a>
                        </td>
                    </tr>*/}
                    <tr>
                        <td>
                            <a style={{ cursor: 'pointer' }} id="about" onClick={()=>{
                                setOverlay(false);
                                props.navigation.push(routeMdw.urlAbout());
                            }}>About</a>
                        </td>
                    </tr>
                </table>
            </div>
        </div>
    </div >
}

export default withStoreProvider(withNavigation(injectIntl(NavBar)));