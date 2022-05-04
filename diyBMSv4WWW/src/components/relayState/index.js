import React, { useState } from 'react';


function RelayState(props) {

    const rule = props.rule;

    return <select id={"rule" + props.ruleId || 0 + "relay" + (props.relayId + 1) || 0} name={"rule" + props.ruleId || 0 + "relay" + (props.relayId + 1) || 0} defaultValue={(() => {
        var relay_value = "X";
        if ((rule.relays[props.relayId] === 0xFF) || (rule.relays[props.relayId] === true) || (rule.relays[props.relayId] == 'On')) { relay_value = "On"; }
        if ((rule.relays[props.relayId] === 0x99) || (rule.relays[props.relayId] === false) || (rule.relays[props.relayId] == 'Off')) { relay_value = "Off"; }
        return relay_value;
    })()}
        onChange={(event) => {
            props.onChange && props.onChange(event.target.value);
        }}>
        <option>On</option>
        <option>Off</option>
        <option>X</option>
    </select>
}

export default RelayState;