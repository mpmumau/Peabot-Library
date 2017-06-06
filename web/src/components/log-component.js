import React, {Component} from 'react';
import {bindActionCreators} from 'redux';
import {connect} from 'react-redux';

import '../../scss/log-component.scss';

class LogComponent extends Component {
    render() {
        return (
            <section className="log-component">
                <ul>
                    <li>
                        <span className="oi" data-glyph="heart"></span>
                        Item 1
                    </li>
                    <li>
                        <span className="oi" data-glyph="heart"></span>
                        Item 2
                    </li>
                    <li>
                        <span className="oi" data-glyph="heart"></span>
                        Item 3
                    </li>
                </ul>
            </section>
        );
    }
}

export default LogComponent;