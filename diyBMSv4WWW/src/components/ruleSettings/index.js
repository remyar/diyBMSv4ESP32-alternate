import React, { useState } from 'react';
import { injectIntl } from 'react-intl';

import { withNavigation } from '../../providers/navigation';
import { withStoreProvider } from '../../providers/StoreProvider';

import RelayState from '../../components/relayState';

function RuleSettings(props) {

    const id = props.id || 0;
    const label = props.label || "";
    const rule = props.rule || {};

    function setRulexvalue(value) {
        rule.value = value;
        props.onChange && props.onChange(rule);
    }

    function setRulexhysteresis(value) {
        rule.hysteresis = value;
        props.onChange && props.onChange(rule);
    }

    function setRulexrelayy(ruleId, relayId, value) {
        rule.relays[relayId] = value;
        props.onChange && props.onChange(rule);
    }

    return <tr>
        <td>
            <label for={"rule" + id + "value"} class={props.triggered ? "triggered" : ""}>{label}</label>
        </td>
        <td>
            <input
                type={props.hidden ? "hidden" : ""}
                name={"rule" + id + "value"}
                id={"rule" + id + "value"}
                min={(props.min && props.min >= 0) ? props.min : -9999999999999}
                max={(props.max && props.max >= 0) ? props.max : 9999999999999}
                step={(props.step && props.step >= 0) ? props.step : 1}
                defaultValue={rule.value}
                onChange={(event) => setRulexvalue(event.target.value)} />
        </td>
        <td>
            <input type={props.hidden ? "hidden" : ""} name={"rule" + id + "hysteresis"} id={"rule" + id + "hysteresis"} defaultValue={rule.hysteresis} onChange={(event) => setRulexhysteresis(event.target.value)} />
        </td>
        <td className="relayset">
            {(() => {
                let _comp = [];
                for (let i = 0; i < 3; i++) {
                    _comp.push(<RelayState
                        ruleId={id}
                        relayId={i}
                        rule={rule}
                        onChange={(val) => {
                            setRulexrelayy(id, i, val);
                        }}
                    />)
                }
                return _comp;
            })()}
        </td>
    </tr>;
}

export default withStoreProvider(withNavigation(injectIntl(RuleSettings)));