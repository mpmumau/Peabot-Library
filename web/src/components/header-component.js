import React, {Component} from 'react';
import {bindActionCreators} from 'redux';
import {connect} from 'react-redux';
import {changePanelAction} from '../actions/actions';

import '../../scss/header-component.scss';

class HeaderComponent extends Component {

    render() {
        return (
            <header>
                <h1>Peabot</h1>
                <ul>
                    <li onClick={() => this.props.changePanelAction("d-pad")}>
                        D-Pad
                    </li>
                    <li onClick={() => this.props.changePanelAction("actions")}>
                        Actions
                    </li>
                    <li onClick={() => this.props.changePanelAction("settings")}>
                        Settings
                    </li>
                    <li className="last" onClick={() => this.props.changePanelAction("log")}>
                        Log
                    </li>
                </ul>
            </header>
        );
    }
};

function mapStateToProps(state) {
    return {}
}

function matchDispatchToProps(dispatch) {
    return bindActionCreators({
        changePanelAction: changePanelAction
    }, dispatch);
}

export default connect(mapStateToProps, matchDispatchToProps)(HeaderComponent);