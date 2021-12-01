import React, { useState } from 'react';


function RelayState(props) {

    const rules = props.rules;
    
    return <select id={"rule" + props.ruleId || 0 + "relay" + (props.relayId+1) || 0} name={"rule" + props.ruleId || 0 + "relay" + (props.relayId+1) || 0} defaultValue={(() => {
        var relay_value = "X";
        if (rules[props.ruleId].relays[props.relayId] === true) { relay_value = "On"; }
        if (rules[props.ruleId].relays[props.relayId] === false) { relay_value = "Off"; }
        return relay_value;
    })()}>
        <option>On</option>
        <option>Off</option>
        <option>X</option>
    </select>
}

export default RelayState;