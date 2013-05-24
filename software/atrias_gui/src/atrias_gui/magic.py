#!/usr/bin/env python

import os
import sys

from qt_gui.plugin_menu import PluginMenu
from qt_gui.main_window import MainWindow
from python_qt_binding.QtGui import QAction

disabled_plugins = [
        "rqt_web/Web",
        "rqt_py_console/PyConsole",
        "rqt_image_view/ImageView",
        "rqt_action/ActionPlugin",
        "rqt_dep/RosPackGraph",
        "rqt_reconfigure/Param",
        "rqt_logger_level/LoggerLevel",
        "rqt_graph/RosGraph",
        "rqt_service_caller/ServiceCaller",
        "rqt_srv/Services",
        "rqt_publisher/Publisher",
        "rqt_msg/Messages",
        "rqt_launch/Launch",
        "rqt_console/Console"
    ]

"""
Our lovely little impostor function
"""
def add_plugin_impostor(current_function):
    def new_func(self, plugin_descriptor):
        if add_plugin_additions(self, plugin_descriptor) == False:
            return
        current_function(self, plugin_descriptor)
    return new_func

def add_plugin_additions(self, plugin_descriptor):
    if plugin_descriptor.plugin_id().startswith('agp_'):
        self.add_plugin_suffix(plugin_descriptor)
    elif plugin_descriptor.plugin_id() in disabled_plugins:
        return False
    #print plugin_descriptor.plugin_id()

"""
Might as well change the name of the window while we're at it >:D
"""
def perspective_changed_impostor(current_function):
    def new_func(self, name):
        current_function(self, name)
        perspective_changed_additions(self, name)
    return new_func

def perspective_changed_additions(self, name):
    self.setWindowTitle('%s - ATRIAS Control Interface 3.0' % str(name))

def add_plugin_suffix(self, plugin_descriptor):
    action_attributes = plugin_descriptor.action_attributes()
    action = QAction(action_attributes['label'], self._plugin_menu_manager.menu)
    self._enrich_action(action, action_attributes)
    self._plugin_mapper.setMapping(action, plugin_descriptor.plugin_id())
    action.triggered.connect(self._plugin_mapper.map)
    self._plugin_menu_manager.add_suffix(action)

def magic(sfile):    
    try:
        import rospkg
        r = rospkg.RosPack()
        p = r.get_path('rqt_gui')
        sys.path.append(os.path.join(p, 'src'))
    except ImportError:
        pass
    
    """
    Here we hamfistedly murder the original add_plugin function.
    We then substitute an impostor function that can make any
    plugins that we don't like disappear.
    """
    PluginMenu.add_plugin = add_plugin_impostor(PluginMenu.add_plugin)
    MainWindow.perspective_changed = perspective_changed_impostor(MainWindow.perspective_changed)
    setattr(PluginMenu, "add_plugin_suffix", add_plugin_suffix)
    
    from rqt_gui.main import Main
    main = Main(os.path.abspath(sfile))
    return main.main()