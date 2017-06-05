import React, {Component} from 'react';
import {bindActionCreators} from 'redux';
import {connect} from 'react-redux';
import {testAction} from '../actions/actions';

import '../../scss/gamepad-component.scss'

class GamepadComponent extends Component {

    constructor(props) {
        super(props);
        this.state = {
            is_moving: false
        };
    }

    testMouseEnter(dir) {
        this.setState({ is_moving: true });
    }

    render() {
        return (
                <section className="gamepad-component">
                    <div className="d-pad">
                        <button className='up' onClick={() => this.props.testAction("up")}>
                            <span className="oi mega" data-glyph="arrow-thick-top"></span>
                        </button>

                        <button className='stop' onClick={() => this.props.testAction("stop")}>
                            <span className="oi mega" data-glyph="target"></span>
                        </button>

                        <button className='down' onClick={() => this.props.testAction("down")}>
                            <span className="oi mega" data-glyph="arrow-thick-bottom"></span>
                        </button>

                        <button className='right' onClick={() => this.props.testAction("right")}>
                            <span className="oi mega" data-glyph="arrow-thick-right"></span>
                        </button>

                        <button className='left' onClick={() => this.props.testAction("left")}>
                            <span className="oi mega" data-glyph="arrow-thick-left"></span>
                        </button>
                    </div>

                    <div className="buttons-panel">
                        <div className="distance">
                            <label>
                                Distance
                            </label>
                            <div className="data-val">
                                123.51cm
                            </div>
                        </div>

                        <div className="speed">
                            <label>
                                Speed
                            </label>
                            <input type="range" name="speed" />
                        </div>                           

                        <div className="rotate-buttons">
                            <button className="left">
                                <span className="oi mega" data-glyph="action-undo"></span>
                            </button>

                            <button className="right">
                                <span className="oi mega" data-glyph="action-redo"></span>
                            </button>                            
                        </div>
                    </div>
                </section>
            );
    }
}

function mapStateToProps(state) {
    if (!state.testReducer || !state.testReducer.first_obj) return {};
    return {
        first_obj: state.testReducer.first_obj
    }
}

function matchDispatchToProps(dispatch) {
    return bindActionCreators({
        testAction: testAction
    }, dispatch);
}

export default connect(mapStateToProps, matchDispatchToProps)(GamepadComponent);