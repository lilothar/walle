#ifndef WALLE_ALGO_ANY_H_
#define WALLE_ALGO_ANY_H_


#include <algorithm>
#include <typeinfo>

namespace std
{
    class any
    {
    public: 

        any()
          : content(0)
        {
        }

        template<typename ValueType>
        any(const ValueType & value)
          : content(new holder<ValueType>(value))
        {
        }

        any(const any & other)
          : content(other.content ? other.content->clone() : 0)
        {
        }

        ~any()
        {
            delete content;
        }

    public: 

        any & swap(any & rhs)
        {
            std::swap(content, rhs.content);
            return *this;
        }

        template<typename ValueType>
        any & operator=(const ValueType & rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

        any & operator=(const any & rhs)
        {
            any(rhs).swap(*this);
            return *this;
        }

    public: 

        bool empty() const
        {
            return !content;
        }

        const std::type_info & type() const
        {
            return content ? content->type() : typeid(void);
        }

    public: 

        class placeholder
        {
        public: // structors
    
            virtual ~placeholder()
            {
            }

        public: // queries

            virtual const std::type_info & type() const = 0;

            virtual placeholder * clone() const = 0;
    
        };

        template<typename ValueType>
        class holder : public placeholder
        {
        public: // structors

            holder(const ValueType & value)
              : held(value)
            {
            }

        public: // queries

            virtual const std::type_info & type() const
            {
                return typeid(ValueType);
            }

            virtual placeholder * clone() const
            {
                return new holder(held);
            }

        public: // representation

            ValueType held;

        };


    public: 
        placeholder * content;

    };

    class bad_any_cast : public std::bad_cast
    {
    public:
        virtual const char * what() const throw()
        {
            return "walle::bad_any_cast: "
                   "failed conversion using walle::any_cast";
        }
    };

    template<typename ValueType>
    ValueType * any_cast(any * operand)
    {
        return operand && operand->type() == typeid(ValueType)
                    ? &static_cast<any::holder<ValueType> *>(operand->content)
->held
                    : 0;
    }

    template<typename ValueType>
    const ValueType * any_cast(const any * operand)
    {
        return any_cast<ValueType>(const_cast<any *>(operand));
    }

    template<typename ValueType>
    ValueType any_cast(const any & operand)
    {
        const ValueType * result = any_cast<ValueType>(&operand);
        if(!result)
            throw bad_any_cast();
        return *result;
    }

}


#endif
