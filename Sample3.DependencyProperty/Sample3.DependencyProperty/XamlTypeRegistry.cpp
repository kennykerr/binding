#include "pch.h"

#include "XamlTypeRegistry.h"

using namespace winrt;

using namespace Windows::Foundation;
using namespace Windows::UI::Xaml::Interop;
using namespace Windows::UI::Xaml::Markup;

struct ITypeRegistry
{
    virtual bool RegisterXamlType(XamlTypeRegistry::TypeInformation const& registration) = 0;

    virtual Windows::UI::Xaml::Markup::IXamlType GetXamlType(hstring const& typeName) = 0;

    virtual ~ITypeRegistry() { };
};

// TODO : why doesn't this work as a std::unique_ptr? It seems to go back to nullptr after getting created. Double initialized?
static ITypeRegistry* s_pTypeRegistry{};

void EnsureRegistry()
{
    struct TypeRegistry : public ITypeRegistry
    {
        TypeRegistry()
        {
            // TODO : only register needed bases
            RegisterPrimitiveType(L"Object");
            RegisterPrimitiveType(L"String");
            RegisterPrimitiveType(L"Windows.UI.Xaml.Controls.Control");
        }

    private:

        struct XamlType : implements<XamlType, IXamlType>
        {
            XamlType(XamlTypeRegistry::TypeInformation const& typeInfo) :
                m_typeInfo(typeInfo)
            {
            }

            hstring FullName() const
            {
                return m_typeInfo.TypeName;
            }

            IInspectable ActivateInstance() const
            {
                return m_typeInfo.CreateInstance();
            }

            IXamlType BaseType() const
            {
                return m_typeInfo.BaseType;
            }

            bool IsConstructible() const
            {
                return (bool)m_typeInfo.CreateInstance;
            }

            TypeName UnderlyingType() const
            {
                return {m_typeInfo.TypeName, m_typeInfo.BaseType ? TypeKind::Custom : TypeKind::Primitive};
            }

            bool IsBindable() const 
            {
                return true; // TODO : if T implements INotifyPropertyChanged
            }

            IXamlMember GetMember(hstring const& name) const
            {
                struct XamlMember : implements<XamlMember, IXamlMember>
                {
                    XamlMember(XamlTypeRegistry::MemberInformation const& memberInfo) :
                        m_memberInfo(memberInfo)
                    {
                    }

                    hstring Name() const
                    {
                        return m_memberInfo.MemberName;
                    }

                    IXamlType Type() const
                    {
                        return m_memberInfo.MemberType;
                    }

                    IInspectable GetValue(IInspectable const& instance) const
                    {
                        return m_memberInfo.Getter(instance);
                    }

                    void SetValue(IInspectable const& instance, IInspectable const& value) const
                    {
                        if (m_memberInfo.Setter)
                        {
                            m_memberInfo.Setter(instance, value);
                        }
                    }

                    bool IsReadOnly() const
                    {
                        return !m_memberInfo.Setter;
                    }

                    bool IsAttachable() const noexcept { return {}; }
                    bool IsDependencyProperty() const noexcept { return {}; }
                    IXamlType TargetType() const noexcept { return {}; }

                private:

                    XamlTypeRegistry::MemberInformation m_memberInfo;
                };

                auto memberInfo = m_typeInfo.GetMember(name);

                if (memberInfo.MemberName == name)
                {
                    return make<XamlMember>(memberInfo);
                }

                return nullptr;
            }

            IXamlMember ContentProperty() const noexcept { return {}; }
            bool IsArray() const noexcept { return {}; }
            bool IsCollection() const noexcept { return {}; }
            bool IsDictionary() const noexcept { return {}; }
            bool IsMarkupExtension() const noexcept { return {}; }
            IXamlType ItemType() const noexcept { return {}; }
            IXamlType KeyType() const noexcept { return {}; }
            IInspectable CreateFromString(hstring const& /*value*/) const noexcept { return {}; }
            void AddToVector(IInspectable const& /*instance*/, IInspectable const& /*value*/) const noexcept { }
            void AddToMap(IInspectable const& /*instance*/, IInspectable const& /*key*/, IInspectable const& /*value*/) const noexcept { }
            void RunInitializer() const noexcept { }

        private:

            XamlTypeRegistry::TypeInformation m_typeInfo;
        };

        virtual bool RegisterXamlType(XamlTypeRegistry::TypeInformation const& registration)
        {
            auto typeName = registration.TypeName;
            if (m_registration.find(typeName) == m_registration.end())
            {
                m_registration[typeName] = registration;
                return true;
            }

            return false;
        }

        virtual IXamlType GetXamlType(hstring const& typeName)
        {
            auto typeEntry = m_typeCache.find(typeName);
            if (typeEntry != m_typeCache.end())
            {
                return typeEntry->second;
            }

            auto registryEntry = m_registration.find(typeName);
            if (registryEntry != m_registration.end())
            {
                auto xamlType = make<XamlType>(registryEntry->second);
                m_typeCache[typeName] = xamlType;
                return xamlType;
            }

            return nullptr;
        }

        void RegisterPrimitiveType(hstring const& typeName)
        {
            XamlTypeRegistry::TypeInformation typeInfo{ typeName };
            m_typeCache[typeName] = make<XamlType>(typeInfo);
        }

        // TODO : can combine into a single collection
        std::map<hstring, XamlTypeRegistry::TypeInformation> m_registration;
        std::map<hstring, IXamlType> m_typeCache;
    };

    if (!s_pTypeRegistry)
    {
        s_pTypeRegistry = new TypeRegistry();
    }
}

bool XamlTypeRegistry::RegisterType(TypeInformation const& typeInfo)
{
    EnsureRegistry();

    return s_pTypeRegistry->RegisterXamlType(typeInfo);
}

IXamlType XamlTypeRegistry::GetType(hstring const& typeName)
{
    EnsureRegistry();

    return s_pTypeRegistry->GetXamlType(typeName);
}