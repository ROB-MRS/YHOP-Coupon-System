#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("system_contract")]] system_contract : public eosio::contract
{

	public:

		using contract::contract;

		/* SMART CONTRACT CONSTRUCTOR */
		system_contract(name receiver, name code, datastream<const char*> ds): contract(receiver, code, ds) {}

		/* ACTION'S IMPLEMENTATION */

		/* COUPON INSERTION METHOD */
		[[eosio::action]]
		void addcoupon(int coupon_id, std::string pub_id, std::string beerlover_id, int value, std::string exp_date)
		{
			coupon_index coupons(get_self(), get_first_receiver().value);

			// verify the existence of the coupon in the table
			auto c_itr = coupons.find(coupon_id);
			check( c_itr == coupons.end() ,"Coupon gia' presente nel sistema!");

			// verify if the 'value' fild is correct
			check( (value<100 && value>0) , "Coupon value out of bound!" );

				// insertion of the record in the table
				coupons.emplace(_self, [&](auto& row) {
					row.coupon_id = coupon_id;
					row.pub_id = pub_id;
					row.beerlover_id = beerlover_id;
					row.value = value;
					row.exp_date = exp_date;
				});

		}


		/* REWARD INSERTION METHOD */
		[[eosio::action]]
		void addreward(int reward_id, std::string hash)
		{
			reward_index rewards(get_self(), get_first_receiver().value);

			// verify the existence of the coupon in the table
			auto r_itr = rewards.find(reward_id);
			check( r_itr == rewards.end() ,"The reward for this coupon already exists!");

			// insertion of the record in the table
			rewards.emplace(_self, [&](auto& row){
				row.reward_id = reward_id;
				row.hash = hash;
			});
		}

		/* COUPON DELETION METHOD */
		[[eosio::action]]
		void erasecoupon(int coupon_id)
		{
			coupon_index coupons(get_self(), get_first_receiver().value);
			
			// search the coupon in the table through the 'id'
			auto c_itr = coupons.find(coupon_id);

			// if the record doesn't exist
			check( c_itr != coupons.end() ,"This coupon doesn't exist!");

			// remove the coupon
			coupons.erase(c_itr);
		}

		/* REWARD DELETION METHOD */
		[[eosio::action]]
		void erasereward(int reward_id)
		{
			reward_index rewards(get_self(), get_first_receiver().value);
			
			// search the coupon in the table through the 'id'
			auto r_itr = rewards.find(reward_id);

			// if the record doesn't exist
			check( r_itr != rewards.end() ,"Il Coupon non esiste!");

			// deletino of the reward
			rewards.erase(r_itr);
		}

	private:

		/* COUPON STRUCT */
		struct [[eosio::table]] coupon {
			int coupon_id;
			std::string pub_id;
			std::string beerlover_id;
			int value;
			std::string exp_date;

			int primary_key() const{ return coupon_id;}
		};

		// coupons table
		using coupon_index = eosio::multi_index<"coupons"_n, coupon>;


		/* REWARD STRUCT */
		struct [[eosio::table]] reward {
			int reward_id;
			std::string hash;

			int primary_key() const{ return reward_id;}
		};

		// rewards table
		using reward_index = eosio::multi_index<"rewards"_n, reward>;

};
