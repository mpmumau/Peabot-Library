import React, {Component} from 'react';
import {bindActionCreators} from 'redux';
import {connect} from 'react-redux';

import '../../scss/settings-panel-component.scss'

class SettingsPanelComponent extends Component {
    render() {
        return (
            <section className="settings-panel-component">
                <div className="left-col">
                    <div className="log-settings settings-section">
                        <label>
                            Log Settings
                        </label>

                        <fieldset>
                            <ul>
                                <li>
                                    <input type="checkbox" name="log-stdin" /> Standard In
                                </li>
                                <li>
                                    <input type="checkbox" name="log-prompt" /> Prompt
                                </li>
                                <li>
                                    <input type="checkbox" name="log-event-add" /> Add Event
                                </li>
                                <li>
                                    <input type="checkbox" name="log-event" /> Event
                                </li>
                                <li>
                                    <input type="checkbox" name="log-keyframes" /> Keyframes
                                </li>
                            </ul>
                            
                        </fieldset>
                    </div>

                    <div className="robot-settings settings-section">
                        <label>
                            Robot
                        </label>

                        <fieldset>
                            <p>
                                <input type="checkbox" name="transitions-enable"/> Enable transitions
                            </p>
                            <p>
                                Transitions time
                                <input type="text" name="transitions-time" />
                            </p>
                        </fieldset>
                    </div>
                </div>

                <div className="right-col">
                    <div className="walk-settings settings-section">
                        <label>
                            Walk Tweaks
                        </label>

                        <fieldset>
                            <p>
                                Hip delta
                                <input type="text" name="hip-delta"/>
                            </p>
                            <p>
                                Knee delta
                                <input type="text" name="knee-delta"/>
                            </p>
                            <p>
                                Knee pad A
                                <input type="text" name="knee-pad-a"/>
                            </p>
                            <p>
                                Knee pad B
                                <input type="text" name="knee-pad-b"/>
                            </p>

                        </fieldset>
                    </div>
                </div>

            </section>
        );
    }
}

export default SettingsPanelComponent;