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
                        <span className="oi" data-glyph="medical-cross"></span> D-Pad
                    </li>
                    <li onClick={() => this.props.changePanelAction("actions")}>
                        <span className="oi" data-glyph="command"></span> Actions
                    </li>
                    <li onClick={() => this.props.changePanelAction("settings")}>
                        <span className="oi" data-glyph="menu"></span> Settings
                    </li>
                    <li className="last" onClick={() => { this.props.changePanelAction("log"); }}>
                        <span className="oi" data-glyph="list"></span> Log
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