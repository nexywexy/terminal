/*++
Copyright (c) Microsoft Corporation
Licensed under the MIT license.

Module Name:
- Command.h

Abstract:
- A command represents a single entry in the Command Palette. This is an object
  that has a user facing "name" to display to the user, and an associated action
  which can be dispatched.

- For more information, see GH#2046, #5400, #5674, and #6635

Author(s):
- Mike Griese - June 2020

--*/
#pragma once

#include "Command.g.h"
#include "TerminalWarnings.h"
#include "Profile.h"
#include "..\inc\cppwinrt_utils.h"
#include "SettingsTypes.h"

// fwdecl unittest classes
namespace SettingsModelLocalTests
{
    class SerializationTests;
    class CommandTests;
};

namespace winrt::Microsoft::Terminal::Settings::Model::implementation
{
    struct Command : CommandT<Command>
    {
        Command();

        static winrt::com_ptr<Command> FromJson(const Json::Value& json,
                                                std::vector<SettingsLoadWarnings>& warnings);

        static void ExpandCommands(Windows::Foundation::Collections::IMap<winrt::hstring, Model::Command> commands,
                                   Windows::Foundation::Collections::IVectorView<Model::Profile> profiles,
                                   Windows::Foundation::Collections::IVectorView<Model::ColorScheme> schemes,
                                   Windows::Foundation::Collections::IVector<SettingsLoadWarnings> warnings);

        static std::vector<SettingsLoadWarnings> LayerJson(Windows::Foundation::Collections::IMap<winrt::hstring, Model::Command>& commands,
                                                           const Json::Value& json);
        bool HasNestedCommands();
        Windows::Foundation::Collections::IMapView<winrt::hstring, Model::Command> NestedCommands();

        void RefreshIcon();

        winrt::Windows::UI::Xaml::Data::INotifyPropertyChanged::PropertyChanged_revoker propertyChangedRevoker;

        WINRT_CALLBACK(PropertyChanged, Windows::UI::Xaml::Data::PropertyChangedEventHandler);
        OBSERVABLE_GETSET_PROPERTY(winrt::hstring, Name, _PropertyChangedHandlers);
        OBSERVABLE_GETSET_PROPERTY(Model::ActionAndArgs, Action, _PropertyChangedHandlers);
        OBSERVABLE_GETSET_PROPERTY(winrt::hstring, KeyChordText, _PropertyChangedHandlers);
        OBSERVABLE_GETSET_PROPERTY(winrt::Windows::UI::Xaml::Controls::IconSource, IconSource, _PropertyChangedHandlers, nullptr);

        GETSET_PROPERTY(ExpandCommandType, IterateOn, ExpandCommandType::None);

    private:
        Json::Value _originalJson;
        Windows::Foundation::Collections::IMap<winrt::hstring, Model::Command> _subcommands{ nullptr };

        winrt::hstring _lastIconPath{};

        static std::vector<Model::Command> _expandCommand(Command* const expandable,
                                                          Windows::Foundation::Collections::IVectorView<Model::Profile> profiles,
                                                          Windows::Foundation::Collections::IVectorView<Model::ColorScheme> schemes,
                                                          Windows::Foundation::Collections::IVector<SettingsLoadWarnings>& warnings);
        friend class SettingsModelLocalTests::SerializationTests;
        friend class SettingsModelLocalTests::CommandTests;
    };
}

namespace winrt::Microsoft::Terminal::Settings::Model::factory_implementation
{
    BASIC_FACTORY(Command);
}
