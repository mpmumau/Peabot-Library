import React, {Component} from 'react';
import {bindActionCreators} from 'redux';
import {connect} from 'react-redux';

import HeaderComponent from './header-component';
import GamepadComponent from './gamepad-component';
import ActionsPanelComponent from './actions-panel-component';
import SettingsPanelComponent from './settings-panel-component';
import LogComponent from './log-component';

class Layout extends Component {
    render() {
        switch (this.props.panel) {
            case "d-pad":
                return (
                    <div>
                        <HeaderComponent />
                        <GamepadComponent />
                    </div>
                );
                break;
            case "actions":
                return (
                    <div>
                        <HeaderComponent />
                        <ActionsPanelComponent />
                    </div>
                );
                break;
            case "settings":
                return (
                    <div>
                        <HeaderComponent />
                        <SettingsPanelComponent />
                    </div>
                );
                break;
            case "log":
                return (
                    <div>
                        <HeaderComponent />
                        <LogComponent />
                    </div>
                );
            default:
                return (
                    <div>
                        <HeaderComponent />
                        <GamepadComponent />
                    </div>
                );
                break;
        }
    }
}

function mapStateToProps(state) {
    if (!state.appState || !state.appState.panel) return {};
    return {
        panel: state.appState.panel
    };
}

export default connect(mapStateToProps)(Layout);