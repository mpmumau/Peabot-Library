import React, {Component} from 'react';
import {bindActionCreators} from 'redux';
import {connect} from 'react-redux';

import HeaderComponent from './header-component';
import GamepadComponent from './gamepad-component';
import ActionsPanelComponent from './actions-panel-component';

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
            default:
                return (
                    <div>
                        <HeaderComponent />
                        <ActionsPanelComponent />
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